/**
 * Non-metric Space Library
 *
 * Authors: Bilegsaikhan Naidan (https://github.com/bileg), Leonid Boytsov (http://boytsov.info).
 * With contributions from Lawrence Cayton (http://lcayton.com/) and others.
 *
 * For the complete list of contributors and further details see:
 * https://github.com/searchivarius/NonMetricSpaceLib 
 * 
 * Copyright (c) 2014
 *
 * This code is released under the
 * Apache License Version 2.0 http://www.apache.org/licenses/.
 *
 */
#ifndef _SPACE_VECTOR_GEN_H_
#define _SPACE_VECTOR_GEN_H_

#include <string>
#include <map>
#include <stdexcept>

#include <space/space_vector.h>

namespace similarity {

/*
 * A generic vector space: can be used with
 * an arbitrary distance function.
 */ 
template <typename dist_t, typename DistObjType>
class VectorSpaceGen : public VectorSpace<dist_t> {
 public:
  virtual ~VectorSpaceGen() {}

  virtual void CreateDataset(ObjectVector& dataset, 
                            const vector<vector<dist_t>>& sourceData) {
    size_t dim = 0;

    for (size_t index = 0; index < sourceData.size(); ++index) {
      const vector<dist_t>& oneElem = sourceData[index];

      size_t currDim = oneElem.size();

      if (!dim) dim = currDim;
      else {
        if (dim != currDim) {
            LOG(LIB_FATAL) << "The # of vector elements (" << currDim << ")" <<
                      " doesn't match the # of elements in previous lines. (" << dim << " )" <<
                      "Found mismatch, index : " << (index + 1);
        }
      }
      dataset.push_back(CreateObjFromVect(index, oneElem));
    }
  }
  virtual std::string ToString() const {
    return "custom space";
  }
  Object* CreateObjFromVect(size_t id, const std::vector<dist_t>& InpVect) const {
    return VectorSpace<dist_t>::CreateObjFromVect(id, InpVect);
  };
 protected:
  DistObjType      distObj_;

  virtual dist_t HiddenDistance(const Object* obj1, const Object* obj2) const {
    CHECK(obj1->datalength() > 0);
    CHECK(obj1->datalength() == obj2->datalength());
    const dist_t* x = reinterpret_cast<const dist_t*>(obj1->data());
    const dist_t* y = reinterpret_cast<const dist_t*>(obj2->data());
    const size_t length = obj1->datalength() / sizeof(dist_t);

    return distObj_(x, y, length);
  }

};

}  // namespace similarity



#endif