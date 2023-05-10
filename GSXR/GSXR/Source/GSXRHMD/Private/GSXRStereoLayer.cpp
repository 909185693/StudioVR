#include "GSXRStereoLayer.h"

FGSXRStereoLayer::FGSXRStereoLayer(uint32 InId, const IStereoLayers::FLayerDesc& InDesc)
	:LayerId(InId)
{
	SetLayerDesc(InDesc);
}

FGSXRStereoLayer::FGSXRStereoLayer(const FGSXRStereoLayer& InLayer)
	: LayerId(InLayer.LayerId)
	, LayerDesc(InLayer.LayerDesc)
{

}

FGSXRStereoLayer::~FGSXRStereoLayer()
{
}

TSharedPtr<FGSXRStereoLayer, ESPMode::ThreadSafe> FGSXRStereoLayer::Clone() const
{
	return MakeShareable(new FGSXRStereoLayer(*this));
}

void FGSXRStereoLayer::SetLayerDesc(const IStereoLayers::FLayerDesc& InDesc)
{
	LayerDesc = InDesc;
}


