// Authors: Olivie Franklova - olivie.abigail.franklova@cern.ch
// Date: 03/2023
// @file HGCalSiliconLayerClusterProducer.cc

#ifndef __RecoLocalCalo_HGCRecProducers_HGCalSiliconLayerClusterProducer_H__
#define __RecoLocalCalo_HGCRecProducers_HGCalSiliconLayerClusterProducer_H__

// user include files

#include "RecoLocalCalo/HGCalRecProducers/plugins/HGCalLayerClusterProducer.h"

using Density = hgcal_clustering::Density;

class HGCalSiliconLayerClusterProducer : public HGCalLayerClusterProducer {
public:
  HGCalSiliconLayerClusterProducer(const edm::ParameterSet& ps
  );
  ~HGCalSiliconLayerClusterProducer() override {}

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
};

DEFINE_FWK_MODULE(HGCalSiliconLayerClusterProducer);

HGCalSiliconLayerClusterProducer::HGCalSiliconLayerClusterProducer(const edm::ParameterSet& ps){
  algoId = reco::CaloCluster::undefined;
  doSharing = ps.getParameter<bool>("doSharing");
  detector = ps.getParameter<std::string>("detector");  // one of EE, FH or "all"
  timeClname = ps.getParameter<std::string>("timeClname");
  timeOffset = ps.getParameter<double>("timeOffset");
  nHitsTime = ps.getParameter<unsigned int>("nHitsTime");
  if (detector == "HFNose") {
    hits_hfnose_token = consumes<HGCRecHitCollection>(ps.getParameter<edm::InputTag>("HFNoseInput"));
    algoId = reco::CaloCluster::hfnose;
  } else if (detector == "all") {
    hits_ee_token = consumes<HGCRecHitCollection>(ps.getParameter<edm::InputTag>("HGCEEInput"));
    hits_fh_token = consumes<HGCRecHitCollection>(ps.getParameter<edm::InputTag>("HGCFHInput"));
    algoId = reco::CaloCluster::hgcal_mixed;
  } else if (detector == "EE") {
    hits_ee_token = consumes<HGCRecHitCollection>(ps.getParameter<edm::InputTag>("HGCEEInput"));
    algoId = reco::CaloCluster::hgcal_em;
  } else if (detector == "FH"){
    hits_fh_token = consumes<HGCRecHitCollection>(ps.getParameter<edm::InputTag>("HGCFHInput"));
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

void HGCalSiliconLayerClusterProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  // hgcalSiliconLayerClusters
  edm::ParameterSetDescription desc;
  edm::ParameterSetDescription pluginDesc;
  pluginDesc.addNode(edm::PluginDescription<HGCalLayerClusterAlgoFactory>("type", "CLUE", true));

  desc.add<edm::ParameterSetDescription>("plugin", pluginDesc);
  desc.add<std::string>("detector", "all")
      ->setComment("all (does not include HFNose); other options: EE, FH, HFNose; default option is undefined");
  desc.add<bool>("doSharing", false);
  desc.add<edm::InputTag>("HFNoseInput", edm::InputTag("HGCalRecHit", "HGCHFNoseRecHits"));
  desc.add<edm::InputTag>("HGCEEInput", edm::InputTag("HGCalRecHit", "HGCEERecHits"));
  desc.add<edm::InputTag>("HGCFHInput", edm::InputTag("HGCalRecHit", "HGCHEFRecHits"));
  desc.add<std::string>("timeClname", "timeLayerCluster");
  desc.add<double>("timeOffset", 0.0);
  desc.add<unsigned int>("nHitsTime", 3);
  descriptions.add("hgcalSiliconLayerClusters", desc);
}


#endif  //__RecoLocalCalo_HGCRecProducers_HGCalSiliconLayerClusterProducer_H__
