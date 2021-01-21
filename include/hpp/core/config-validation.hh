//
// Copyright (c) 2014 CNRS
// Authors: Florent Lamiraux
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

#ifndef HPP_CORE_CONFIG_VALIDATION_HH
# define HPP_CORE_CONFIG_VALIDATION_HH

# include <hpp/core/validation-report.hh>
# include <hpp/core/relative-motion.hh>
# include <hpp/core/deprecated.hh>

namespace hpp {
  namespace core {
    /// \addtogroup validation
    /// \{

    /// Abstraction of configuration validation
    ///
    /// Instances of this class validate configurations with respect to some
    /// criteria
    class HPP_CORE_DLLAPI ConfigValidation
    {
    public:
      /// Compute whether the configuration is valid
      ///
      /// \param config the config to check for validity,
      /// \retval validationReport report on validation. If non valid,
      ///         a validation report will be allocated and returned via this
      ///         shared pointer.
      /// \return whether the whole config is valid.
      virtual bool validate (const Configuration_t& config,
			     ValidationReportPtr_t& validationReport) = 0;

      virtual ~ConfigValidation () = default;

    protected:
      ConfigValidation () = default;
    }; // class ConfigValidation
    /// \}
  } // namespace core
} // namespace hpp

#endif // HPP_CORE_CONFIG_VALIDATION_HH
