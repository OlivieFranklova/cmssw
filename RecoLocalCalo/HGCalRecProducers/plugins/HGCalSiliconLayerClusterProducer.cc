// Authors: Olivie Franklova - olivie.abigail.franklova@cern.ch
// Date: 03/2023
// @file HGCalSiliconLayerClusterProducer.cc represents cluster producer for silicon layer 

#ifndef __RecoLocalCalo_HGCRecProducers_HGCalSiliconLayerClusterProducer_H__
#define __RecoLocalCalo_HGCRecProducers_HGCalSiliconLayerClusterProducer_H__

// user include files

#include "RecoLocalCalo/HGCalRecProducers/plugins/HGCalLayerClusterProducer.h"

using Density = hgcal_clustering::Density;

/**
 * @brief Silicon Layer cluster producer is child of Layer cluster producer
 * 
 *  represents producer only for silicon layers and has method fillDescription
*/
class HGCalSiliconLayerClusterProducer : public HGCalLayerClusterProducer {
public:
  HGCalSiliconLayerClusterProducer(const edm::ParameterSet& ps
  );
  ~HGCalSiliconLayerClusterProducer() override {}

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
};

DEFINE_FWK_MODULE(HGCalSiliconLayerClusterProducer);

/**
   * @brief Constructor with parameter set. Constructor will set all variables by input param ps. 
   * token and algoID variables will be set accordingly to the detector type, but only for detector EE and FH
   * 
   * @param[in] ps parametr set to set variables
*/
HGCalSiliconLayerClusterProducer::HGCalSiliconLayerClusterProducer(const edm::ParameterSet& ps){
  algoId = reco::CaloCluster::undefined;
  doSharing = ps.getParameter<bool>("doSharing");
  detector = ps.getParameter<std::string>("detector");  // one of EE, FH 
  timeClname = ps.getParameter<std::string>("timeClname");
  timeOffset = ps.getParameter<double>("timeOffset");
  nHitsTime = ps.getParameter<unsigned int>("nHitsTime");
  if (detector == "EE") {
    hits_ee_token = consumes<HGCRecHitCollection>(ps.getParameter<edm::InputTag>("HGCEEInput"));
    algoId = reco::CaloCluster::hgcal_em;
  } else {
    hits_fh_token = consumes<HGCRecHitCollection>(ps.getParameter<edm::InputTag>("HGCFHInput"));
    algoId = reco::CaloCluster::hgcal_had;
  }

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
void HGCalSiliconLayerClusterProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  // hgcalSiliconLayerClusters
  edm::ParameterSetDescription desc;
  edm::ParameterSetDescription pluginDesc;
  pluginDesc.addNode(edm::PluginDescription<HGCalLayerClusterAlgoFactory>("type", "CLUE", true)); //todo add specific algo

  desc.add<edm::ParameterSetDescription>("plugin", pluginDesc);
  desc.add<std::string>("detector", "FH")
      ->setComment("options: EE, FH; default option is FH");
  desc.add<bool>("doSharing", false);
  desc.add<edm::InputTag>("HGCEEInput", edm::InputTag("HGCalRecHit", "HGCEERecHits"));
  desc.add<edm::InputTag>("HGCFHInput", edm::InputTag("HGCalRecHit", "HGCHEFRecHits"));
  desc.add<std::string>("timeClname", "timeLayerCluster");
  desc.add<double>("timeOffset", 0.0);
  desc.add<unsigned int>("nHitsTime", 3);
  descriptions.add("hgcalSiliconLayerClusters", desc);
}


#endif  //__RecoLocalCalo_HGCRecProducers_HGCalSiliconLayerClusterProducer_H__
