import FWCore.ParameterSet.Config as cms

filteredLayerClustersTrkEM = cms.EDProducer("FilteredLayerClustersProducer",
    LayerClusters = cms.InputTag("hgcalMergeLayerClusters"),
    LayerClustersInputMask = cms.InputTag("hgcalMergeLayerClusters","InitialLayerClustersMask"),
    # algo_number = cms.int32(8),
    algo_number = cms.vint32(6,7),
    clusterFilter = cms.string('ClusterFilterByAlgoAndSizeAndLayerRange'),
    iteration_label = cms.string('TrkEM'),
    max_cluster_size = cms.int32(9999),
    max_layerId = cms.int32(30),
    mightGet = cms.optional.untracked.vstring,
    min_cluster_size = cms.int32(3),
    min_layerId = cms.int32(0)
)
