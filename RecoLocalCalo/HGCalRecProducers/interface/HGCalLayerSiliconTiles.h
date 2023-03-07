// Authors: Olivie Franklova - olivie.abigail.franklova@cern.ch
// Date: 03/2023

#ifndef RecoLocalCalo_HGCalRecProducers_HGCalLayerSiliconTiles_h
#define RecoLocalCalo_HGCalRecProducers_HGCalLayerSiliconTiles_h

#include "RecoLocalCalo/HGCalRecProducers/interface/HGCalTilesConstants.h"
#include "RecoLocalCalo/HGCalRecProducers/interface/HFNoseTilesConstants.h"
#include "DataFormats/Math/interface/normalizedPhi.h"

#include <vector>
#include <array>
#include <cmath>
#include <algorithm>
#include <cassert>

template <typename T>
class HGCalLayerSiliconTilesT {
public:
  typedef T type;
  void fill(const std::vector<float>& x,
            const std::vector<float>& y,
            ) {
    auto cellsSize = x.size();
    for (unsigned int i = 0; i < cellsSize; ++i) {
      tiles_[getGlobalBin(x[i], y[i])].push_back(i);
    }
  }

  int getXBin(float x) const {
    constexpr float xRange = T::maxX - T::minX; 
    static_assert(xRange >= 0.);
    constexpr float r = T::nColumns / xRange;
    int xBin = (x - T::minX) * r;
    xBin = std::clamp(xBin, 0, T::nColumns - 1);
    return xBin;
  }

  int getYBin(float y) const {
    constexpr float yRange = T::maxY - T::minY;
    static_assert(yRange >= 0.);
    constexpr float r = T::nRows / yRange;
    int yBin = (y - T::minY) * r;
    yBin = std::clamp(yBin, 0, T::nRows - 1);
    return yBin;
  }


  int getGlobalBin(float x, float y) const { return getXBin(x) + getYBin(y) * T::nColumns; }

  int getGlobalBinByBin(int xBin, int yBin) const { return xBin + yBin * T::nColumns; }


  std::array<int, 4> searchBox(float xMin, float xMax, float yMin, float yMax) const {
    int xBinMin = getXBin(xMin);
    int xBinMax = getXBin(xMax);
    int yBinMin = getYBin(yMin);
    int yBinMax = getYBin(yMax);
    return std::array<int, 4>({{xBinMin, xBinMax, yBinMin, yBinMax}});
  }


  void clear() {
    for (auto& t : tiles_)
      t.clear();
  }

  const std::vector<int>& operator[](int globalBinId) const { return tiles_[globalBinId]; }

private:
  std::array<std::vector<int>, T::nTiles> tiles_;
};

using HGCalLayerSiliconTiles = HGCalLayerSiliconTilesT<HGCalTilesConstants>;
using HFNoseLayerSiliconTiles = HGCalLayerSiliconTilesT<HFNoseTilesConstants>;
#endif