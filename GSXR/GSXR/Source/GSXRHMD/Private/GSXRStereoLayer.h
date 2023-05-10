
#pragma once
#include "IStereoLayers.h"

class FGSXRStereoLayer : public TSharedFromThis<FGSXRStereoLayer, ESPMode::ThreadSafe>
{
public:
	FGSXRStereoLayer(uint32 InId, const IStereoLayers::FLayerDesc& InDesc);
	FGSXRStereoLayer(const FGSXRStereoLayer& InLayer);
	~FGSXRStereoLayer();
	
	TSharedPtr<FGSXRStereoLayer, ESPMode::ThreadSafe> Clone() const;
	void SetLayerDesc(const IStereoLayers::FLayerDesc& InDesc);
	const IStereoLayers::FLayerDesc& GetLayerDesc() const { return LayerDesc; }
	const uint32& GetLayerID()const{return LayerId;}
	uint32 LayerId;
	IStereoLayers::FLayerDesc LayerDesc;

};

typedef TSharedPtr<FGSXRStereoLayer, ESPMode::ThreadSafe> FGSXRLayerPtr;