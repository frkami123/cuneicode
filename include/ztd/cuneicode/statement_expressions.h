// ============================================================================
//
// ztd.cuneicode
// Copyright © 2022-2023 JeanHeyd "ThePhD" Meneide and Shepherd's Oasis, LLC
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

#ifndef ZTD_CUNEICODE_STATEMENT_EXPRESSIONS_H
#define ZTD_CUNEICODE_STATEMENT_EXPRESSIONS_H

#pragma once

#include <ztd/cuneicode/version.h>

#include <ztd/cuneicode.h>
#include <ztd/cuneicode/result.h>

#include <ztd/idk/size.h>

//////
/// @addtogroup ztd_cuneicode_statement_expressions Statement Expression-based Convenience Functions
///
/// @{

#define cnc_xntoyn(_INPUT_SIZE, _INPUT, _SUFFIX)                  \
	({                                                           \
		typeof(_INPUT) __input_ptr          = (_INPUT);         \
		cnc_single_result(_SUFFIX) __result = { 0 };            \
		cnc_mcstate_t __state               = { 0 };            \
		__result.error                      = cnc_cxsntocysn(); \
		__result;                                               \
	})

#define cnc_xntomc(_INPUT_SIZE, _INPUT)

#define cnc_Xtomc(_INPUT) cnc_Xntomc(ztdc_c_string_ptr_size(_INPUT), _INPUT)



//////
/// @}

#endif
