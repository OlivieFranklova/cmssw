// Authors: Olivie Franklova - olivie.abigail.franklova@cern.ch
// Date: 03/2023

#ifndef RecoLocalCalo_HGCalRecProducers_HGCalLayerScintillatorTiles_h
#define RecoLocalCalo_HGCalRecProducers_HGCalLayerScintillatorTiles_h

#include "RecoLocalCalo/HGCalRecProducers/interface/HGCalTilesScintillatorConstants.h"
#include "RecoLocalCalo/HGCalRecProducers/interface/HFNoseTilesScintillatorConstants.h"
#include "DataFormats/Math/interface/normalizedPhi.h"

#include <vector>
#include <array>
#include <cmath>
#include <algorithm>
#include <cassert>

template <typename T>
class HGCalLayerScintillatorTilesT {
public:
  typedef T type;
  void fill(const std::vector<float>& eta,
            const std::vector<float>& phi) {
    auto cellsSize = eta.size(); 
    for (unsigned int i = 0; i < cellsSize; ++i) {
      tiles_[getGlobalBinEtaPhi(eta[i], phi[i])].push_back(i);
    }
  }

  int getEtaBin(float eta) const {
    constexpr float etaRange = T::maxEta - T::minEta;
    static_assert(etaRange >= 0.);
    constexpr float r = T::nColumnsEta / etaRange;
    int etaBin = (eta - T::minEta) * r;
    etaBin = std::clamp(etaBin, 0, T::nColumnsEta - 1);
    return etaBin;
  }

  int getPhiBin(float phi) const {
    auto normPhi = normalizedPhi(phi);
    constexpr float r = T::nRowsPhi * M_1_PI * 0.5f;
    int phiBin = (normPhi + M_PI) * r;
    return phiBin;
  }

  int mPiPhiBin = getPhiBin(-M_PI);
  int pPiPhiBin = getPhiBin(M_PI);

  int getGlobalBinEtaPhi(float eta, float phi) const {
    return T::nColumns * T::nRows + getEtaBin(eta) + getPhiBin(phi) * T::nColumnsEta;
  }

  int getGlobalBinByBinEtaPhi(int etaBin, int phiBin) const {
    return T::nColumns * T::nRows + etaBin + phiBin * T::nColumnsEta;
  }


  std::array<int, 4> searchBoxEtaPhi(float etaMin, float etaMax, float phiMin, float phiMax) const {
    if (etaMax - etaMin < 0) {
      return std::array<int, 4>({{0, 0, 0, 0}});
    }
    int etaBinMin = getEtaBin(etaMin);
    int etaBinMax = getEtaBin(etaMax);
    int phiBinMin = getPhiBin(phiMin);
    int phiBinMax = getPhiBin(phiMax);
    // If the search window cross the phi-bin boundary, add T::nPhiBins to the
    // MAx value. This guarantees that the caller can perform a valid doule
    // loop on eta and phi. It is the caller responsibility to perform a module
    // operation on the phiBin values returned by this function, to explore the
    // correct bins.
    if (phiBinMax < phiBinMin) {
      phiBinMax += T::nRowsPhi;
    }

    return std::array<int, 4>({{etaBinMin, etaBinMax, phiBinMin, phiBinMax}});
  }

  void clear() {
    for (auto& t : tiles_)
      t.clear();
  }

  const std::vector<int>& operator[](int globalBinId) const { return tiles_[globalBinId]; }

private:
  std::array<std::vector<int>, T::nTiles> tiles_;
};

using HGCalLayerScintillatorTiles = HGCalLayerScintillatorTilesT<HGCalTilesConstants>;
using HFNoseLayerScintillatorTiles = HGCalLayerScintillatorTilesT<HFNoseTilesConstants>;
#endif
