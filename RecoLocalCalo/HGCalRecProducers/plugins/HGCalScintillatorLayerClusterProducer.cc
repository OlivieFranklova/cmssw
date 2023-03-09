// Authors: Olivie Franklova - olivie.abigail.franklova@cern.ch
// Date: 03/2023
// @file HGCalScintillatorLayerClusterProducer.cc represents cluster producer for scintilator layer 

#ifndef __RecoLocalCalo_HGCRecProducers_HGCalScintillatorLayerClusterProducer_H__
#define __RecoLocalCalo_HGCRecProducers_HGCalScintillatorLayerClusterProducer_H__

// user include files

#include "RecoLocalCalo/HGCalRecProducers/plugins/HGCalLayerClusterProducer.h"

using Density = hgcal_clustering::Density;

class HGCalScintillatorLayerClusterProducer : public HGCalLayerClusterProducer {
public:
  HGCalScintillatorLayerClusterProducer(const edm::ParameterSet& ps);
  ~HGCalScintillatorLayerClusterProducer() override {}
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
};

DEFINE_FWK_MODULE(HGCalScintillatorLayerClusterProducer);

HGCalScintillatorLayerClusterProducer::HGCalScintillatorLayerClusterProducer(const edm::ParameterSet& ps){
  algoId = reco::CaloCluster::undefined;
  doSharing = ps.getParameter<bool>("doSharing");
  detector = ps.getParameter<std::string>("detector");  // one of BH or "all"
  timeClname = ps.getParameter<std::string>("timeClname");
  timeOffset = ps.getParameter<double>("timeOffset");
  nHitsTime = ps.getParameter<unsigned int>("nHitsTime");
  if (detector == "HFNose") {
    hits_hfnose_token = consumes<HGCRecHitCollection>(ps.getParameter<edm::InputTag>("HFNoseInput"));
    algoId = reco::CaloCluster::hfnose;
  } else if (detector == "all") { 
    hits_bh_token = consumes<HGCRecHitCollection>(ps.getParameter<edm::InputTag>("HGCBHInput"));
    algoId = reco::CaloCluster::hgcal_mixed;
  } else if (detector == "BH") {
    hits_bh_token = consumes<HGCRecHitCollection>(ps.getParameter<edm::InputTag>("HGCBHInput"));
    algoId = reco::CaloCluster::hgcal_had;
  }
  else{
     algoId = reco::CaloCluster::undefined;
  }

  auto pluginPSet = ps.getParameter<edm::ParameterSet>("plugin");
  if (detector == "HFNose") {
    algo = HGCalLayerClusterAlgoFactory::get()->create("HFNoseCLUE", pluginPSet, consumesCollector());
    algo->setAlgoId(algoId, true);
  } else {
    algo = HGCalLayerClusterAlgoFactory::get()->create(
        pluginPSet.getParameter<std::string>("type"), pluginPSet, consumesCollector());
    algo->setAlgoId(algoId);
  }

  produces<std::vector<float>>("InitialLayerClustersMask");
  produces<std::vector<reco::BasicCluster>>();
  produces<std::vector<reco::BasicCluster>>("sharing");
  //density
  produces<Density>();
  //time for layer clusters
  produces<edm::ValueMap<std::pair<float, float>>>(timeClname);
}

void HGCalScintillatorLayerClusterProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  // hgcalScintillatorLayerClusters
  edm::ParameterSetDescription desc;
  edm::ParameterSetDescription pluginDesc;
  pluginDesc.addNode(edm::PluginDescription<HGCalLayerClusterAlgoFactory>("type", "CLUE", true));

  desc.add<edm::ParameterSetDescription>("plugin", pluginDesc);
  desc.add<std::string>("detector", "all")
      ->setComment("all (does not include HFNose); other options: BH, HFNose; default option is undefined");
  desc.add<bool>("doSharing", false);
  desc.add<edm::InputTag>("HFNoseInput", edm::InputTag("HGCalRecHit", "HGCHFNoseRecHits"));
  desc.add<edm::InputTag>("HGCBHInput", edm::InputTag("HGCalRecHit", "HGCHEBRecHits"));
  desc.add<std::string>("timeClname", "timeLayerCluster");
  desc.add<double>("timeOffset", 0.0);
  desc.add<unsigned int>("nHitsTime", 3);
  descriptions.add("hgcalScintillatorLayerClusters", desc);
}


#endif  //__RecoLocalCalo_HGCRecProducers_HGCalScintillatorLayerClusterProducer_H__
