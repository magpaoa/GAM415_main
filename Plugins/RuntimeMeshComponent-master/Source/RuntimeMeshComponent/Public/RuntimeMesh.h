// Copyright 2016-2020 Chris Conway (Koderz). All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RuntimeMeshCore.h"
#include "RuntimeMeshCollision.h"
#include "RuntimeMeshProvider.h"
#include "RuntimeMesh.generated.h"

class URuntimeMesh;
class URuntimeMeshComponent;

class FRuntimeMeshData;
using FRuntimeMeshDataPtr = TSharedPtr<FRuntimeMeshData, ESPMode::ThreadSafe>;
class FRuntimeMeshProxy;
using FRuntimeMeshProxyPtr = TSharedPtr<FRuntimeMeshProxy, ESPMode::ThreadSafe>;

class URuntimeMeshProviderStatic;

/**
*	Delegate for when the collision was updated.
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRuntimeMeshCollisionUpdatedDelegate);


UCLASS(HideCategories = Object, BlueprintType)
class RUNTIMEMESHCOMPONENT_API URuntimeMesh : public UObject, public IInterface_CollisionDataProvider
{
	GENERATED_UCLASS_BODY()

private:
	/** 
	*	Whether this mesh needs to be initialized by the tick object. 
	*	This is to get away from postload so BP calls in the 
	*	provider are safe 
	*/
	bool bNeedsInitialization;

	/** Reference to the underlying data object */
	FRuntimeMeshDataPtr Data;

	/** All RuntimeMeshComponents linked to this mesh. Used to alert the components of changes */
	TArray<TWeakObjectPtr<URuntimeMeshComponent>> LinkedComponents;

	UPROPERTY()
	URuntimeMeshProvider* MeshProvider;

	/** Do we need to update our collision? */
	bool bCollisionIsDirty;



	/** Collision data */
	UPROPERTY(Instanced)
	UBodySetup* BodySetup;
	TArray<FRuntimeMeshCollisionSourceSectionInfo> CollisionSource;

	/** Queue of pending collision cooks */
	UPROPERTY(Transient)
	TArray<FRuntimeMeshAsyncBodySetupData> AsyncBodySetupQueue;

	TUniquePtr<TArray<FRuntimeMeshCollisionSourceSectionInfo>> PendingSourceInfo;
public:

	UFUNCTION(BlueprintCallable)
	void Initialize(URuntimeMeshProvider* Provider);

	UFUNCTION(BlueprintCallable)
	void Reset();

	/** Creates a static provider replacing whatever provider exists. */
	UFUNCTION(BlueprintCallable)
	URuntimeMeshProviderStatic* InitializeStaticProvider();

	UFUNCTION(BlueprintCallable)
	URuntimeMeshProvider* GetProvider() { return MeshProvider; }

	FRuntimeMeshProviderProxyPtr GetCurrentProviderProxy();
	
	UFUNCTION(BlueprintCallable)
	TArray<FRuntimeMeshMaterialSlot> GetMaterialSlots() const;

	UFUNCTION(BlueprintCallable)
	int32 GetNumMaterials();
	
	UFUNCTION(BlueprintCallable)
	UMaterialInterface* GetMaterial(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	void SetupMaterialSlot(int32 MaterialSlot, FName SlotName, UMaterialInterface* InMaterial);

	UFUNCTION(BlueprintCallable)
	int32 GetMaterialIndex(FName MaterialSlotName) const;

	UFUNCTION(BlueprintCallable)
	TArray<FName> GetMaterialSlotNames() const;

	UFUNCTION(BlueprintCallable)
	bool IsMaterialSlotNameValid(FName MaterialSlotName) const;

	UFUNCTION(BlueprintCallable)
	FBoxSphereBounds GetLocalBounds() const;

	UBodySetup* GetBodySetup() { return nullptr;  }
	
	/** Event called when the collision has finished updated, this works both with standard following frame synchronous updates, as well as async updates */
	UPROPERTY(BlueprintAssignable, Category = "Components|RuntimeMesh")
	FRuntimeMeshCollisionUpdatedDelegate CollisionUpdated;

	/* This is to get a copy of the current lod/section configuration */
	TArray<FRuntimeMeshLOD, TInlineAllocator<RUNTIMEMESH_MAXLODS>> GetCopyOfConfiguration() const;

	static void InitializeMultiThreading(int32 NumThreads, int32 StackSize = 0, EThreadPriority ThreadPriority = TPri_BelowNormal);

	static FRuntimeMeshBackgroundWorkDelegate InitializeUserSuppliedThreading();


	void BeginDestroy() override;

private:
	FRuntimeMeshCollisionHitInfo GetHitSource(int32 FaceIndex) const;

private:
	/** Triggers a rebuild of the collision data on the next tick */
	void MarkCollisionDirty();

	/** Helper to create new body setup objects */
	UBodySetup* CreateNewBodySetup();
	/** Mark collision data as dirty, and re-create on instance if necessary */
	void UpdateCollision(bool bForceCookNow = false);
	/** Once async physics cook is done, create needed state, and then call the user event */

#if ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION >= 21
	void FinishPhysicsAsyncCook(bool bSuccess, UBodySetup* FinishedBodySetup);
#else
	void FinishPhysicsAsyncCook(UBodySetup* FinishedBodySetup);
#endif
	/** Runs all post cook tasks like alerting the user event and alerting linked components */
	void FinalizeNewCookedData();

protected:
	bool GetPhysicsTriMeshData(struct FTriMeshCollisionData* CollisionData, bool InUseAllTriData) override;
	bool ContainsPhysicsTriMeshData(bool InUseAllTriData) const override;
private:

	void InitializeInternal();

	void RegisterLinkedComponent(URuntimeMeshComponent* NewComponent);
	void UnRegisterLinkedComponent(URuntimeMeshComponent* ComponentToRemove);
	bool GetSceneFeatureLevel(ERHIFeatureLevel::Type& OutFeatureLevel);


	void EnsureReadyToRender(ERHIFeatureLevel::Type InFeatureLevel);
	FRuntimeMeshProxyPtr GetRenderProxy(ERHIFeatureLevel::Type InFeatureLevel);

	template<typename Function>
	void DoForAllLinkedComponents(Function Func)
	{
		bool bShouldPurge = false;
		for (TWeakObjectPtr<URuntimeMeshComponent> MeshReference : LinkedComponents)
		{
			if (URuntimeMeshComponent * Mesh = MeshReference.Get())
			{
				Func(Mesh);
			}
			else
			{
				bShouldPurge = true;
			}
		}
		if (bShouldPurge)
		{
			LinkedComponents = LinkedComponents.FilterByPredicate([](const TWeakObjectPtr<URuntimeMeshComponent>& MeshReference)
				{
					return MeshReference.IsValid();
				});
		}
	}


	void UpdateAllComponentsBounds();
	void RecreateAllComponentProxies();

	virtual void MarkChanged();

	void PostLoad();


	friend class URuntimeMeshComponent;
	friend class FRuntimeMeshComponentSceneProxy;
	friend class FRuntimeMeshData;
	friend struct FRuntimeMeshDelayedActionTickObject;
};

