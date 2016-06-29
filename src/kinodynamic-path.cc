// Copyright (c) 2016, LAAS-CNRS
// Authors: Pierre Fernbach (pierre.fernbach@laas.fr)
//
// This file is part of hpp-core
// hpp-core is free software: you can redistribute it
// and/or modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation, either version
// 3 of the License, or (at your option) any later version.
//
// hpp-core is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Lesser Public License for more details.  You should have
// received a copy of the GNU Lesser General Public License along with
// hpp-core  If not, see
// <http://www.gnu.org/licenses/>.

#include <hpp/util/debug.hh>
#include <hpp/model/device.hh>
#include <hpp/model/configuration.hh>
#include <hpp/core/config-projector.hh>
#include <hpp/core/kinodynamic-path.hh>
#include <hpp/core/projection-error.hh>

namespace hpp {
  namespace core {
    KinodynamicPath::KinodynamicPath (const DevicePtr_t& device,
                                      ConfigurationIn_t init,
                                      ConfigurationIn_t end,
                                      value_type length, ConfigurationIn_t a1, ConfigurationIn_t t1, ConfigurationIn_t tv, ConfigurationIn_t t2, double vMax) :
      parent_t (interval_t (0, length), device->configSize (),
                device->numberDof ()),
      device_ (device), initial_ (init), end_ (end),a1_(a1),t1_(t1),tv_(tv),t2_(t2),vMax_(vMax)
    {
      assert (device);
      assert (length >= 0);
      assert (!constraints ());
      hppDout(notice,"Create kinodynamic path with values : ");
      hppDout(notice,"a1 = "<<model::displayConfig(a1_));
      hppDout(notice,"t1 = "<<model::displayConfig(t1_));
      hppDout(notice,"tv = "<<model::displayConfig(tv_));
      hppDout(notice,"t2 = "<<model::displayConfig(t2_));
      hppDout(notice,"vMax = "<<vMax_);
      
      
    }
    
    KinodynamicPath::KinodynamicPath (const DevicePtr_t& device,
                                      ConfigurationIn_t init,
                                      ConfigurationIn_t end,
                                      value_type length, ConfigurationIn_t a1, ConfigurationIn_t t1, ConfigurationIn_t tv, ConfigurationIn_t t2, double vMax,
                                      ConstraintSetPtr_t constraints) :
      parent_t (interval_t (0, length), device->configSize (),
                device->numberDof (), constraints),
      device_ (device), initial_ (init), end_ (end),a1_(a1),t1_(t1),tv_(tv),t2_(t2),vMax_(vMax)
    {
      assert (device);
      assert (length >= 0);
    }
    
    KinodynamicPath::KinodynamicPath (const KinodynamicPath& path) :
      parent_t (path), device_ (path.device_), initial_ (path.initial_),
      end_ (path.end_)
    {
    }
    
    KinodynamicPath::KinodynamicPath (const KinodynamicPath& path,
                                      const ConstraintSetPtr_t& constraints) :
      parent_t (path, constraints), device_ (path.device_),
      initial_ (path.initial_), end_ (path.end_)
    {
      assert (constraints->apply (initial_));
      assert (constraints->apply (end_));
      assert (constraints->isSatisfied (initial_));
      assert (constraints->isSatisfied (end_));
    }
    
    bool KinodynamicPath::impl_compute (ConfigurationOut_t result,
                                        value_type param) const
    {
      if (param == timeRange ().first || timeRange ().second == 0) {
        result = initial_;
        return true;
      }
      if (param == timeRange ().second) {
        result = end_;
        return true;
      }
      
      const JointVector_t& jv (problem_->robot()->getJointVector ());
      for (model::JointVector_t::const_iterator itJoint = jv.begin (); itJoint != jv.end (); itJoint++) {
        size_type indexConfig = (*itJoint)->rankInConfiguration ();
        size_type indexVel = (*itJoint)->rankInVelocity() + configSize;
        hppDout(notice," PATH For joint "<<(*itJoint)->name());
        if((*itJoint)->configSize() >= 1){
          
        }
        
      }// for all joints
      
      return true;
    }
    
    PathPtr_t KinodynamicPath::extract (const interval_t& subInterval) const
    throw (projection_error)
    {
      // Length is assumed to be proportional to interval range
      value_type l = fabs (subInterval.second - subInterval.first);
      
      bool success;
      Configuration_t q1 ((*this) (subInterval.first, success));
      if (!success) throw projection_error
          ("Failed to apply constraints in KinodynamicPath::extract");
      Configuration_t q2 ((*this) (subInterval.second, success));
      if (!success) throw projection_error
          ("Failed to apply constraints in KinodynamicPath::extract");
      PathPtr_t result = KinodynamicPath::create (device_, q1, q2, l,a1_,t1_,tv_,t2_,vMax_,
                                                  constraints ());
      return result;
    }
    
    DevicePtr_t KinodynamicPath::device () const
    {
      return device_;
    }
  } //   namespace core
} // namespace hpp
