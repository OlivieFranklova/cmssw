// Authors: Olivie Franklova - olivie.abigail.franklova@cern.ch
// Date: 03/2023
// @file HGCalMixLayerClusterProducer.cc represents cluster producer for both silicon and scintilator layer 

#ifndef __RecoLocalCalo_HGCRecProducers_HGCalMixLayerClusterProducer_H__
#define __RecoLocalCalo_HGCRecProducers_HGCalMixLayerClusterProducer_H__

// user include files

#include "RecoLocalCalo/HGCalRecProducers/plugins/HGCalLayerClusterProducer.h"

using Density = hgcal_clustering::Density;

/**
 * @brief Mix Layer cluster producer is child of Layer cluster producer
 * 
 *  represents producer for both silicon and scintilator layers and has method fillDescription
*/
class HGCalMixLayerClusterProducer : public HGCalLayerClusterProducer {
public:
  HGCalMixLayerClusterProducer(const edm::ParameterSet& ps
  );
  ~HGCalMixLayerClusterProducer() override {}

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
};

DEFINE_FWK_MODULE(HGCalMixLayerClusterProducer);

/**
   * @brief Constructor with parameter set. Constructor will set all variables by input param ps. 
   * token and algoID variables will be set accordingly to all detector types - EE, FH and BH
   * 
   * @param[in] ps parametr set to set variables
*/
HGCalMixLayerClusterProducer::HGCalMixLayerClusterProducer(const edm::ParameterSet& ps){
  algoId = reco::CaloCluster::undefined;
  doSharing = ps.getParameter<bool>("doSharing");
  detector = ps.getParameter<std::string>("detector");  // should be all
  timeClname = ps.getParameter<std::string>("timeClname");
  timeOffset = ps.getParameter<double>("timeOffset");
  nHitsTime = ps.getParameter<unsigned int>("nHitsTime");
  hits_ee_token = consumes<HGCRecHitCollection>(ps.getParameter<edm::InputTag>("HGCEEInput"));
  hits_fh_token = consumes<HGCRecHitCollection>(ps.getParameter<edm::InputTag>("HGCFHInput"));
  hits_bh_token = consumes<HGCRecHitCollection>(ps.getParameter<edm::InputTag>("HGCBHInput"));
  algoId = reco::CaloCluster::hgcal_mixed;

    //todo make function from this
  auto pluginPSet = ps.getParameter<edm::ParameterSet>("plugin");
  algo = HGCalLayerClusterAlgoFactory::get()->create(
     pluginPSet.getParameter<std::string>("type"), pluginPSet, consumesCollector());
  algo->setAlgoId(algoId);

  setUpProduces();
}

/**
   * @brief Method fill description which will be used for generation pyhton file.
   * 
   * @param[out] description to be fill
*/
void HGCalMixLayerClusterProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  // hgcalMixLayerClusters
  edm::ParameterSetDescription desc;
  edm::ParameterSetDescription pluginDesc;
  pluginDesc.addNode(edm::PluginDescription<HGCalLayerClusterAlgoFactory>("type", "CLUE", true));

  desc.add<edm::ParameterSetDescription>("plugin", pluginDesc);
  desc.add<std::string>("detector", "all")
      ->setComment("all includes EE, FH, BH");
  desc.add<bool>("doSharing", false);
  desc.add<edm::InputTag>("HGCEEInput", edm::InputTag("HGCalRecHit", "HGCEERecHits"));
  desc.add<edm::InputTag>("HGCFHInput", edm::InputTag("HGCalRecHit", "HGCHEFRecHits"));
  desc.add<edm::InputTag>("HGCBHInput", edm::InputTag("HGCalRecHit", "HGCHEBRecHits"));
  desc.add<std::string>("timeClname", "timeLayerCluster");
  desc.add<double>("timeOffset", 0.0);
  desc.add<unsigned int>("nHitsTime", 3);
  descriptions.add("hgcalMixLayerClusters", desc);
}


#endif  //__RecoLocalCalo_HGCRecProducers_HGCalMixLayerClusterProducer_H__
