// This file is part of libigl, a simple c++ geometry processing library.
//
// Copyright (C) 2014 Daniele Panozzo <daniele.panozzo@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
#include "igl/dot_row.h"

template <typename DerivedV>
IGL_INLINE Eigen::PlainObjectBase<DerivedV> igl::dot_row(
  const Eigen::PlainObjectBase<DerivedV>& A,
  const Eigen::PlainObjectBase<DerivedV>& B
  )
{
  assert(A.rows() == B.rows());
  assert(A.cols() == B.cols());

  return (A.array() * B.array()).rowwise().sum();
}

#ifdef IGL_STATIC_LIBRARY
// Explicit template specialization
// generated by autoexplicit.sh
template Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > igl::dot_row<Eigen::Matrix<double, -1, -1, 0, -1, -1> >(Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&);
#endif
