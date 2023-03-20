import FWCore.ParameterSet.Config as cms

from ..modules.filteredLayerClustersCLUE3DHighL1Seeded_cfi import *
from ..modules.hgcalDigis_cfi import *
from ..modules.hgcalDigisL1Seeded_cfi import *
from ..modules.hgcalLayerClustersL1Seeded_cfi import *
from ..modules.HGCalRecHitL1Seeded_cfi import *
from ..modules.HGCalUncalibRecHitL1Seeded_cfi import *
from ..modules.hltL1TEGammaHGCFilteredCollectionProducer_cfi import *
from ..modules.hltRechitInRegionsHGCAL_cfi import *
from ..modules.offlineBeamSpot_cfi import *
from ..modules.particleFlowClusterHGCalFromTICLL1Seeded_cfi import *
from ..modules.particleFlowRecHitHGCL1Seeded_cfi import *
from ..modules.particleFlowSuperClusterHGCalFromTICLL1Seeded_cfi import *
from ..modules.ticlLayerTileProducerL1Seeded_cfi import *
from ..modules.ticlSeedingL1_cfi import *
from ..modules.ticlTrackstersCLUE3DHighL1Seeded_cfi import *
from RecoLocalCalo.HGCalRecProducers.hgcalMergeLayerClusters_cfi import hgcalMergeLayerClusters

hgcalMergeLayerClustersL1Seeded = hgcalMergeLayerClusters.clone()
hgcalMergeLayerClustersL1Seeded.layerClustersEE = cms.InputTag('hgcalLayerClusters', 'hgcalLayerClustersEEL1Seeded')
hgcalMergeLayerClustersL1Seeded.layerClustersHSi = cms.InputTag('hgcalLayerClusters', 'hgcalLayerClustersHSiL1Seeded')
hgcalMergeLayerClustersL1Seeded.layerClustersHSci = cms.InputTag('hgcalLayerClusters', 'hgcalLayerClustersHSciL1Seeded')

HLTHgcalTiclPFClusteringForEgammaL1SeededTask = cms.Task(
    HGCalRecHitL1Seeded,
    HGCalUncalibRecHitL1Seeded,
    filteredLayerClustersCLUE3DHighL1Seeded,
    hgcalDigis,
    hgcalDigisL1Seeded,
    hgcalLayerClustersEEL1Seeded,
    hgcalLayerClustersHSiL1Seeded,
    hgcalLayerClustersHSciL1Seeded,
    hgcalMergeLayerClustersL1Seeded,
    hltL1TEGammaHGCFilteredCollectionProducer,
    hltRechitInRegionsHGCAL,
    offlineBeamSpot,
    particleFlowClusterHGCalFromTICLL1Seeded,
    particleFlowRecHitHGCL1Seeded,
    particleFlowSuperClusterHGCalFromTICLL1Seeded,
    ticlLayerTileProducerL1Seeded,
    ticlSeedingL1,
    ticlTrackstersCLUE3DHighL1Seeded
)
