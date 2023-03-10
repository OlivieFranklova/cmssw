// @file HGCalLayerClusterProducer.h represents producer for layer cluster
#ifndef __RecoLocalCalo_HGCRecProducers_HGCalLayerClusterProducer_H__
#define __RecoLocalCalo_HGCRecProducers_HGCalLayerClusterProducer_H__

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/ParameterSet/interface/PluginDescription.h"

#include "RecoParticleFlow/PFClusterProducer/interface/RecHitTopologicalCleanerBase.h"
#include "RecoParticleFlow/PFClusterProducer/interface/SeedFinderBase.h"
#include "RecoParticleFlow/PFClusterProducer/interface/InitialClusteringStepBase.h"
#include "RecoParticleFlow/PFClusterProducer/interface/PFClusterBuilderBase.h"
#include "RecoParticleFlow/PFClusterProducer/interface/PFCPositionCalculatorBase.h"
#include "RecoParticleFlow/PFClusterProducer/interface/PFClusterEnergyCorrectorBase.h"
#include "RecoLocalCalo/HGCalRecProducers/interface/ComputeClusterTime.h"

#include "RecoLocalCalo/HGCalRecProducers/interface/HGCalLayerClusterAlgoFactory.h"
#include "RecoLocalCalo/HGCalRecAlgos/interface/HGCalDepthPreClusterer.h"

#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/HGCalGeometry/interface/HGCalGeometry.h"

#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/Common/interface/ValueMap.h"

using Density = hgcal_clustering::Density;
/**
 * @brief Layer cluster producer is child of EDProducer 
 * 
 * class overides method produce and has method fillDescription
*/
class HGCalLayerClusterProducer : public edm::stream::EDProducer<> {
public:
  /**
   * @brief Constructor with parameter set. Constructor will set all variables by input param ps. 
   * token and algoID variables will be set accordingly to the detector type.
   * 
   * @param[in] ps parametr set to set variables
  */
  HGCalLayerClusterProducer(const edm::ParameterSet& ps);
  HGCalLayerClusterProducer() {};
  ~HGCalLayerClusterProducer() override {}
  /**
   * @brief Method fill description which will be used in pyhton file.
   * 
   * @param[out] description to be fill
  */
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  /**
   * @brief Method run the algoritm to get clusters.
   * 
   * @param[in, out] evt from get info and put result
   * @param[in] es to get event setup info
  */
  void produce(edm::Event& evt, const edm::EventSetup& es) override;

protected:
  edm::EDGetTokenT<HGCRecHitCollection> hits_ee_token;
  edm::EDGetTokenT<HGCRecHitCollection> hits_fh_token;
  edm::EDGetTokenT<HGCRecHitCollection> hits_bh_token;
  edm::EDGetTokenT<HGCRecHitCollection> hits_hfnose_token;

  reco::CaloCluster::AlgoId algoId;

  std::unique_ptr<HGCalClusteringAlgoBase> algo;
  bool doSharing;
  std::string detector;

  std::string timeClname;
  double timeOffset;
  unsigned int nHitsTime;

  /**
  * @brief this method populate algo for specific hits and then fill hitmap
  * method is used in produce in switch
  */
  void produceForAlgoId(edm::Handle<HGCRecHitCollection>& hits, std::unordered_map<uint32_t, const HGCRecHit*>& hitmap ){
    algo->populate(*hits);
    for (auto const& it : *hits)
      hitmap[it.detid().rawId()] = &(it);
  }

  /**
  * @brief this method do all set up for produces, method is used in constructor
  */
  void setUpProduces(){
    produces<std::vector<float>>("InitialLayerClustersMask");
    produces<std::vector<reco::BasicCluster>>();
    produces<std::vector<reco::BasicCluster>>("sharing");
    //density
    produces<Density>();
    //time for layer clusters
    produces<edm::ValueMap<std::pair<float, float>>>(timeClname);
  }


};

#endif  //__RecoLocalCalo_HGCRecProducers_HGCalLayerClusterProducer_H__
