// ============================================================================
//
// ztd.cuneicode
// Copyright © JeanHeyd "ThePhD" Meneide and Shepherd's Oasis, LLC
// Contact: opensource@soasis.org
//
// Commercial License Usage
// Licensees holding valid commercial ztd.cuneicode licenses may use this file
// in accordance with the commercial license agreement provided with the
// Software or, alternatively, in accordance with the terms contained in
// a written agreement between you and Shepherd's Oasis, LLC.
// For licensing terms and conditions see your agreement. For
// further information contact opensource@soasis.org.
//
// Apache License Version 2 Usage
// Alternatively, this file may be used under the terms of Apache License
// Version 2.0 (the "License"); you may not use this file except in compliance
// with the License. You may obtain a copy of the License at
//
// https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// ========================================================================= //

#pragma once

#ifndef ZTD_CUNEICODE_SOURCE_DETAIL_ERR_HPP
#define ZTD_CUNEICODE_SOURCE_DETAIL_ERR_HPP

#include <ztd/cuneicode/version.h>

#include <cstddef>

namespace cnc {
	ZTD_CUNEICODE_INLINE_ABI_NAMESPACE_OPEN_I_
	namespace __cnc_detail {

		inline constexpr const size_t __stdc_ret_null_value            = 0;
		inline constexpr const size_t __stdc_ret_no_write_out_yet      = 0;
		inline constexpr const size_t __stdc_ret_err_invalid           = static_cast<size_t>(-1);
		inline constexpr const size_t __stdc_ret_err_incomplete_input  = static_cast<size_t>(-2);
		inline constexpr const size_t __stdc_ret_accumulated_write_out = static_cast<size_t>(-3);

	} // namespace __cnc_detail
	ZTD_CUNEICODE_INLINE_ABI_NAMESPACE_CLOSE_I_
} // namespace cnc

#endif
