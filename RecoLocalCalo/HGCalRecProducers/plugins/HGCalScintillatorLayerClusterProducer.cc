// Authors: Olivie Franklova - olivie.abigail.franklova@cern.ch
// Date: 03/2023
// @file HGCalScintillatorLayerClusterProducer.cc represents cluster producer for scintilator layer 

#ifndef __RecoLocalCalo_HGCRecProducers_HGCalScintillatorLayerClusterProducer_H__
#define __RecoLocalCalo_HGCRecProducers_HGCalScintillatorLayerClusterProducer_H__

// user include files

#include "RecoLocalCalo/HGCalRecProducers/plugins/HGCalLayerClusterProducer.h"

using Density = hgcal_clustering::Density;

/**
 * @brief Scintillator Layer cluster producer is child of Layer cluster producer
 * 
 *  represents producer only for scintillator layers and has method fillDescription
*/
class HGCalScintillatorLayerClusterProducer : public HGCalLayerClusterProducer {
public:
  HGCalScintillatorLayerClusterProducer(const edm::ParameterSet& ps);
  ~HGCalScintillatorLayerClusterProducer() override {}
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
};

DEFINE_FWK_MODULE(HGCalScintillatorLayerClusterProducer);

/**
   * @brief Constructor with parameter set. Constructor will set all variables by input param ps. 
   * token and algoID variables will be set accordingly to the detector type, but only for detector BH
   * 
   * @param[in] ps parametr set to set variables
*/
HGCalScintillatorLayerClusterProducer::HGCalScintillatorLayerClusterProducer(const edm::ParameterSet& ps){
  algoId = reco::CaloCluster::undefined;
  doSharing = ps.getParameter<bool>("doSharing");
  detector = ps.getParameter<std::string>("detector");  // one of BH 
  timeClname = ps.getParameter<std::string>("timeClname");
  timeOffset = ps.getParameter<double>("timeOffset");
  nHitsTime = ps.getParameter<unsigned int>("nHitsTime");
  hits_bh_token = consumes<HGCRecHitCollection>(ps.getParameter<edm::InputTag>("HGCBHInput"));
  algoId = reco::CaloCluster::hgcal_had;


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
void HGCalScintillatorLayerClusterProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  // hgcalScintillatorLayerClusters
  edm::ParameterSetDescription desc;
  edm::ParameterSetDescription pluginDesc;
  pluginDesc.addNode(edm::PluginDescription<HGCalLayerClusterAlgoFactory>("type", "CLUE", true)); //todo change to the right algo

  desc.add<edm::ParameterSetDescription>("plugin", pluginDesc);
  desc.add<std::string>("detector", "BH")
      ->setComment("default option is BH");
  desc.add<bool>("doSharing", false);
  desc.add<edm::InputTag>("HGCBHInput", edm::InputTag("HGCalRecHit", "HGCHEBRecHits"));
  desc.add<std::string>("timeClname", "timeLayerCluster");
  desc.add<double>("timeOffset", 0.0);
  desc.add<unsigned int>("nHitsTime", 3);
  descriptions.add("hgcalScintillatorLayerClusters", desc);
}


#endif  //__RecoLocalCalo_HGCRecProducers_HGCalScintillatorLayerClusterProducer_H__
