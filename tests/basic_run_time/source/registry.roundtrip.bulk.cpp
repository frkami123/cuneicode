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

#include <catch2/catch_all.hpp>

#include <ztd/cuneicode.h>

#include <ztd/cuneicode/shared/unicode_range.hpp>

#include <iterator>
#include <algorithm>
#include <cstring>

namespace {

	template <typename Char, typename Input>
	void compare_bulk_roundtrip_utf32(
	     cnc_conversion* from_conv, cnc_conversion* to_conv, Input& input_buffer) {
		const size_t initial_input_size = (input_buffer.size() * sizeof(*input_buffer.data()));
		const unsigned char* const initial_input
		     = reinterpret_cast<const unsigned char*>(input_buffer.data());
		const unsigned char* const initial_input_end = initial_input + initial_input_size;
		const unsigned char* input                   = initial_input;
		size_t input_size                            = initial_input_size;
		std::vector<unsigned char> output_buffer(initial_input_size * 5 * 4);
		unsigned char* output            = output_buffer.data();
		const size_t initial_output_size = output_buffer.size();
		size_t output_size               = initial_output_size;

		const cnc_mcerr output_err
		     = cnc_conv(from_conv, &output_size, &output, &input_size, &input);
		const size_t input_read     = initial_input_size - input_size;
		const size_t output_written = initial_output_size - output_size;
		REQUIRE(output_err == cnc_mcerr_ok);
		REQUIRE(input_read == initial_input_size);
		REQUIRE(input_size == 0);
		output_buffer.resize(output_written);

		{
			// validate input
			const unsigned char* initial_validate_input = initial_input;
			const unsigned char* validate_input         = initial_validate_input;
			size_t validate_input_size                  = initial_input_size;
			const cnc_mcerr validate_input_err
			     = cnc_conv(from_conv, nullptr, nullptr, &validate_input_size, &validate_input);
			REQUIRE(validate_input_err == cnc_mcerr_ok);
			REQUIRE((validate_input - initial_validate_input)
			     == (static_cast<std::ptrdiff_t>(input_read)));
		}
		{
			// validate input: direct
			const unsigned char* initial_validate_input = initial_input;
			const unsigned char* validate_input         = initial_validate_input;
			size_t validate_input_size                  = initial_input_size;
			const bool validate_input_is_valid
			     = cnc_conv_is_valid(from_conv, &validate_input_size, &validate_input);
			REQUIRE(validate_input_is_valid);
			REQUIRE((validate_input - initial_validate_input)
			     == (static_cast<std::ptrdiff_t>(input_read)));
		}
		{
			// count output
			const unsigned char* initial_count_input = initial_input;
			const unsigned char* count_input         = initial_count_input;
			size_t count_input_size                  = initial_input_size;
			size_t count_output_size                 = initial_output_size;
			const cnc_mcerr count_input_err          = cnc_conv(
                    from_conv, &count_output_size, nullptr, &count_input_size, &count_input);
			const size_t count_output_written = initial_output_size - count_output_size;
			REQUIRE(count_input_err == cnc_mcerr_ok);
			REQUIRE((count_input - initial_count_input)
			     == (static_cast<std::ptrdiff_t>(input_read)));
			REQUIRE(count_output_written == output_written);
		}
		{
			// count output: direct
			const unsigned char* initial_count_input = initial_input;
			const unsigned char* count_input         = initial_count_input;
			size_t count_input_size                  = initial_input_size;
			size_t count_output_size                 = initial_output_size;
			const cnc_mcerr count_input_err          = cnc_conv_count(
                    from_conv, &count_output_size, &count_input_size, &count_input);
			const size_t count_output_written = initial_output_size - count_output_size;
			REQUIRE(count_input_err == cnc_mcerr_ok);
			REQUIRE((count_input - initial_count_input)
			     == (static_cast<std::ptrdiff_t>(input_read)));
			REQUIRE(count_output_written == output_written);
		}
		{
			// unbounded output
			const unsigned char* initial_unbounded_input = initial_input;
			const unsigned char* unbounded_input         = initial_unbounded_input;
			size_t unbounded_input_size                  = initial_input_size;
			std::vector<unsigned char> unbounded_output_buffer(output_written);
			unsigned char* initial_unbounded_output = unbounded_output_buffer.data();
			unsigned char* unbounded_output         = initial_unbounded_output;
			const cnc_mcerr unbounded_input_err     = cnc_conv(
                    from_conv, nullptr, &unbounded_output, &unbounded_input_size, &unbounded_input);
			REQUIRE(unbounded_input_err == cnc_mcerr_ok);
			REQUIRE((unbounded_input - initial_unbounded_input)
			     == (static_cast<std::ptrdiff_t>(input_read)));
			REQUIRE((unbounded_output - initial_unbounded_output)
			     == (static_cast<std::ptrdiff_t>(output_written)));
			REQUIRE((initial_unbounded_output + output_written) == unbounded_output);
			REQUIRE(std::equal(std::cbegin(unbounded_output_buffer),
			     std::cend(unbounded_output_buffer), std::cbegin(output_buffer),
			     std::cend(output_buffer)));
		}

		{
			// unbounded output: direct
			const unsigned char* initial_unbounded_input = initial_input;
			const unsigned char* unbounded_input         = initial_unbounded_input;
			size_t unbounded_input_size                  = initial_input_size;
			std::vector<unsigned char> unbounded_output_buffer(output_written);
			unsigned char* initial_unbounded_output = unbounded_output_buffer.data();
			unsigned char* unbounded_output         = initial_unbounded_output;
			const cnc_mcerr unbounded_input_err     = cnc_conv_unbounded(
                    from_conv, &unbounded_output, &unbounded_input_size, &unbounded_input);
			REQUIRE(unbounded_input_err == cnc_mcerr_ok);
			REQUIRE((unbounded_input - initial_unbounded_input)
			     == (static_cast<std::ptrdiff_t>(input_read)));
			REQUIRE((unbounded_output - initial_unbounded_output)
			     == (static_cast<std::ptrdiff_t>(output_written)));
			REQUIRE((initial_unbounded_output + output_written) == unbounded_output);
			REQUIRE(std::equal(std::cbegin(unbounded_output_buffer),
			     std::cend(unbounded_output_buffer), std::cbegin(output_buffer),
			     std::cend(output_buffer)));
		}

		const unsigned char* initial_intermediate = output_buffer.data();
		const unsigned char* intermediate         = initial_intermediate;
		const size_t initial_intermediate_size    = output_written;
		size_t intermediate_size                  = initial_intermediate_size;
		std::vector<unsigned char> input_output_buffer(initial_input_size);
		unsigned char* initial_input_output    = input_output_buffer.data();
		unsigned char* input_output            = initial_input_output;
		const size_t initial_input_output_size = input_output_buffer.size();
		size_t input_output_size               = initial_input_output_size;
		const cnc_mcerr input_output_err       = cnc_conv(
               to_conv, &input_output_size, &input_output, &intermediate_size, &intermediate);
		const size_t intermediate_read    = initial_intermediate_size - intermediate_size;
		const size_t input_output_written = initial_input_output_size - input_output_size;
		REQUIRE(input_output_err == cnc_mcerr_ok);
		REQUIRE(intermediate_size == 0);
		REQUIRE(intermediate_read == output_written);
		REQUIRE(input_output_written == initial_input_size);
		REQUIRE(input_output_written == input_read);
		REQUIRE((initial_intermediate + intermediate_read) == intermediate);
		REQUIRE((initial_intermediate_size - intermediate_read) == intermediate_size);
		REQUIRE((initial_input_output + input_output_written) == input_output);
		REQUIRE((initial_input_output_size - input_output_written) == input_output_size);
		// resize only after doing all the pointer-testing shenanigans
		input_output_buffer.resize(input_output_written);
		REQUIRE(std::equal(initial_input, initial_input_end, std::cbegin(input_output_buffer),
		     std::cend(input_output_buffer)));

		{
			// validate intermediate
			const unsigned char* initial_validate_input = initial_intermediate;
			const unsigned char* validate_input         = initial_validate_input;
			size_t validate_input_size                  = initial_intermediate_size;
			const cnc_mcerr validate_input_err
			     = cnc_conv(to_conv, nullptr, nullptr, &validate_input_size, &validate_input);
			REQUIRE(validate_input_err == cnc_mcerr_ok);
			REQUIRE((validate_input - initial_validate_input)
			     == (static_cast<std::ptrdiff_t>(intermediate_read)));
		}


		{
			// validate intermediate: direct
			const unsigned char* initial_validate_input = initial_intermediate;
			const unsigned char* validate_input         = initial_validate_input;
			size_t validate_input_size                  = initial_intermediate_size;
			const bool validate_input_is_valid
			     = cnc_conv_is_valid(to_conv, &validate_input_size, &validate_input);
			REQUIRE(validate_input_is_valid);
			REQUIRE((validate_input - initial_validate_input)
			     == (static_cast<std::ptrdiff_t>(intermediate_read)));
		}
		{
			// count output
			const unsigned char* initial_count_input = initial_intermediate;
			const unsigned char* count_input         = initial_count_input;
			size_t count_input_size                  = initial_intermediate_size;
			size_t count_output_size                 = initial_input_output_size;
			const cnc_mcerr count_input_err          = cnc_conv(
                    to_conv, &count_output_size, nullptr, &count_input_size, &count_input);
			const size_t count_output_written = initial_input_output_size - count_output_size;
			REQUIRE(count_input_err == cnc_mcerr_ok);
			REQUIRE((count_input - initial_count_input)
			     == (static_cast<std::ptrdiff_t>(intermediate_read)));
			REQUIRE(count_output_written == input_output_written);
		}
		{
			// count output: direct
			const unsigned char* initial_count_input = initial_intermediate;
			const unsigned char* count_input         = initial_count_input;
			size_t count_input_size                  = initial_intermediate_size;
			size_t count_output_size                 = initial_input_output_size;
			const cnc_mcerr count_input_err
			     = cnc_conv_count(to_conv, &count_output_size, &count_input_size, &count_input);
			const size_t count_output_written = initial_input_output_size - count_output_size;
			REQUIRE(count_input_err == cnc_mcerr_ok);
			REQUIRE((count_input - initial_count_input)
			     == (static_cast<std::ptrdiff_t>(intermediate_read)));
			REQUIRE(count_output_written == input_output_written);
		}
		{
			// unbounded output
			const unsigned char* initial_unbounded_input = initial_intermediate;
			const unsigned char* unbounded_input         = initial_unbounded_input;
			size_t unbounded_input_size                  = initial_intermediate_size;
			std::vector<unsigned char> unbounded_output_buffer(input_output_written);
			unsigned char* initial_unbounded_output = unbounded_output_buffer.data();
			unsigned char* unbounded_output         = initial_unbounded_output;
			const cnc_mcerr unbounded_input_err     = cnc_conv(
                    to_conv, nullptr, &unbounded_output, &unbounded_input_size, &unbounded_input);
			REQUIRE(unbounded_input_err == cnc_mcerr_ok);
			REQUIRE((unbounded_input - initial_unbounded_input)
			     == (static_cast<std::ptrdiff_t>(intermediate_read)));
			REQUIRE((unbounded_output - initial_unbounded_output)
			     == (static_cast<std::ptrdiff_t>(input_output_written)));
			REQUIRE((initial_unbounded_output + input_output_written) == unbounded_output);
			REQUIRE(std::equal(std::cbegin(unbounded_output_buffer),
			     std::cend(unbounded_output_buffer), std::cbegin(input_output_buffer),
			     std::cend(input_output_buffer)));
		}
		{
			// unbounded output: direct
			const unsigned char* initial_unbounded_input = initial_intermediate;
			const unsigned char* unbounded_input         = initial_unbounded_input;
			size_t unbounded_input_size                  = initial_intermediate_size;
			std::vector<unsigned char> unbounded_output_buffer(input_output_written);
			unsigned char* initial_unbounded_output = unbounded_output_buffer.data();
			unsigned char* unbounded_output         = initial_unbounded_output;
			const cnc_mcerr unbounded_input_err     = cnc_conv_unbounded(
                    to_conv, &unbounded_output, &unbounded_input_size, &unbounded_input);
			REQUIRE(unbounded_input_err == cnc_mcerr_ok);
			REQUIRE((unbounded_input - initial_unbounded_input)
			     == (static_cast<std::ptrdiff_t>(intermediate_read)));
			REQUIRE((unbounded_output - initial_unbounded_output)
			     == (static_cast<std::ptrdiff_t>(input_output_written)));
			REQUIRE((initial_unbounded_output + input_output_written) == unbounded_output);
			REQUIRE(std::equal(std::cbegin(unbounded_output_buffer),
			     std::cend(unbounded_output_buffer), std::cbegin(input_output_buffer),
			     std::cend(input_output_buffer)));
		}
	}

} // namespace

TEST_CASE(
     "check bulk conversion from one of the typical typed encodings to UTF-32 and back to the "
     "typed encoding, using the registry",
     "[cuneicode][registry][roundtrip-c32][bulk]") {
	const auto& unicode_input         = full_unicode_range();
	const auto& basic_source_input    = basic_source_range();
	cnc_conversion_registry* registry = NULL;
	cnc_open_err reg_err              = cnc_registry_new(&registry, cnc_registry_options_default);
	REQUIRE(reg_err == cnc_open_err_ok);
	SECTION("mc") {
		const ztd_char8_t* from_name  = (const ztd_char8_t*)u8"utf32";
		const ztd_char8_t* to_name    = (const ztd_char8_t*)u8"execution";
		cnc_conversion* from_conv     = NULL;
		cnc_conversion* to_conv       = NULL;
		cnc_conversion_info from_info = {};
		cnc_conversion_info to_info   = {};
		cnc_open_err from_err
		     = cnc_conv_new_c8(registry, from_name, to_name, &from_conv, &from_info);
		cnc_open_err to_err = cnc_conv_new_c8(registry, to_name, from_name, &to_conv, &to_info);
		REQUIRE(from_err == cnc_open_err_ok);
		REQUIRE(to_err == cnc_open_err_ok);
		compare_bulk_roundtrip_utf32<ztd_char_t>(from_conv, to_conv, basic_source_input);
		if (cnc_is_execution_encoding_unicode()) {
			compare_bulk_roundtrip_utf32<ztd_char_t>(from_conv, to_conv, unicode_input);
		}
		cnc_conv_close(from_conv);
		cnc_conv_close(to_conv);
	}
	SECTION("mwc") {
		const ztd_char8_t* from_name  = (const ztd_char8_t*)u8"utf32";
		const ztd_char8_t* to_name    = (const ztd_char8_t*)u8"wide-execution";
		cnc_conversion* from_conv     = NULL;
		cnc_conversion* to_conv       = NULL;
		cnc_conversion_info from_info = {};
		cnc_conversion_info to_info   = {};
		cnc_open_err from_err
		     = cnc_conv_new_c8(registry, from_name, to_name, &from_conv, &from_info);
		cnc_open_err to_err = cnc_conv_new_c8(registry, to_name, from_name, &to_conv, &to_info);
		REQUIRE(from_err == cnc_open_err_ok);
		REQUIRE(to_err == cnc_open_err_ok);
		compare_bulk_roundtrip_utf32<ztd_wchar_t>(from_conv, to_conv, basic_source_input);
		if (cnc_is_wide_execution_encoding_unicode()) {
			compare_bulk_roundtrip_utf32<ztd_wchar_t>(from_conv, to_conv, unicode_input);
		}
		cnc_conv_close(from_conv);
		cnc_conv_close(to_conv);
	}
	SECTION("c8") {
		const ztd_char8_t* from_name  = (const ztd_char8_t*)u8"utf32";
		const ztd_char8_t* to_name    = (const ztd_char8_t*)u8"utf8";
		cnc_conversion* from_conv     = NULL;
		cnc_conversion* to_conv       = NULL;
		cnc_conversion_info from_info = {};
		cnc_conversion_info to_info   = {};
		cnc_open_err from_err
		     = cnc_conv_new_c8(registry, from_name, to_name, &from_conv, &from_info);
		cnc_open_err to_err = cnc_conv_new_c8(registry, to_name, from_name, &to_conv, &to_info);
		REQUIRE(from_err == cnc_open_err_ok);
		REQUIRE(to_err == cnc_open_err_ok);
		compare_bulk_roundtrip_utf32<ztd_char8_t>(from_conv, to_conv, basic_source_input);
		compare_bulk_roundtrip_utf32<ztd_char8_t>(from_conv, to_conv, unicode_input);
		cnc_conv_close(from_conv);
		cnc_conv_close(to_conv);
	}
	SECTION("c16") {
		const ztd_char8_t* from_name  = (const ztd_char8_t*)u8"utf32";
		const ztd_char8_t* to_name    = (const ztd_char8_t*)u8"utf16";
		cnc_conversion* from_conv     = NULL;
		cnc_conversion* to_conv       = NULL;
		cnc_conversion_info from_info = {};
		cnc_conversion_info to_info   = {};
		cnc_open_err from_err
		     = cnc_conv_new_c8(registry, from_name, to_name, &from_conv, &from_info);
		cnc_open_err to_err = cnc_conv_new_c8(registry, to_name, from_name, &to_conv, &to_info);
		REQUIRE(from_err == cnc_open_err_ok);
		REQUIRE(to_err == cnc_open_err_ok);
		compare_bulk_roundtrip_utf32<ztd_char16_t>(from_conv, to_conv, basic_source_input);
		compare_bulk_roundtrip_utf32<ztd_char16_t>(from_conv, to_conv, unicode_input);
		cnc_conv_close(from_conv);
		cnc_conv_close(to_conv);
	}
	SECTION("c32") {
		const ztd_char8_t* from_name  = (const ztd_char8_t*)u8"utf32";
		const ztd_char8_t* to_name    = (const ztd_char8_t*)u8"utf32";
		cnc_conversion* from_conv     = NULL;
		cnc_conversion* to_conv       = NULL;
		cnc_conversion_info from_info = {};
		cnc_conversion_info to_info   = {};
		cnc_open_err from_err
		     = cnc_conv_new_c8(registry, from_name, to_name, &from_conv, &from_info);
		cnc_open_err to_err = cnc_conv_new_c8(registry, to_name, from_name, &to_conv, &to_info);
		REQUIRE(from_err == cnc_open_err_ok);
		REQUIRE(to_err == cnc_open_err_ok);
		compare_bulk_roundtrip_utf32<ztd_char32_t>(from_conv, to_conv, basic_source_input);
		compare_bulk_roundtrip_utf32<ztd_char32_t>(from_conv, to_conv, unicode_input);
		cnc_conv_close(from_conv);
		cnc_conv_close(to_conv);
	}
	cnc_registry_delete(registry);
}
