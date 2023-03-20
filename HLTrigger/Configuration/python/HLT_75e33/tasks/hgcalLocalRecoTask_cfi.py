import FWCore.ParameterSet.Config as cms

from ..modules.hgcalLayerClusters_cfi import *
from RecoLocalCalo.HGCalRecProducers.hgcalMergeLayerClusters_cfi import hgcalMergeLayerClusters
# from RecoLocalCalo.HGCalRecProducers.hgcalLayerClusters_cff import hgcalLayerClustersHFNose, hgcalLayerClustersEE, hgcalLayerClustersHSi, hgcalLayerClustersHSci
from ..modules.HGCalRecHit_cfi import *
from ..modules.HGCalUncalibRecHit_cfi import *
from ..modules.particleFlowClusterHGCal_cfi import *
from ..modules.particleFlowRecHitHGC_cfi import *

hgcalLocalRecoTask = cms.Task(
    HGCalRecHit,
    HGCalUncalibRecHit,
    hgcalLayerClustersEE,
    hgcalLayerClustersHSi,
    hgcalLayerClustersHSci,
    hgcalMergeLayerClusters,
    particleFlowClusterHGCal,
    particleFlowRecHitHGC
)
