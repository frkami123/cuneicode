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

#include <cconv/usage.hpp>
#include <cconv/def.hpp>

#include <set>
#include <iostream>

struct encoding_data {
	utf8string_view name;
	bool from  = false;
	bool to    = false;
	bool alias = false;

	friend constexpr bool operator==(
	     const encoding_data& left, const encoding_data& right) noexcept {
		return left.name == right.name;
	}

	friend constexpr bool operator<(
	     const encoding_data& left, const encoding_data& right) noexcept {
		return left.name < right.name;
	}
};

void print_encoding_list(cnc_conversion_registry* p_registry) {
	struct user_data_t {
		std::set<encoding_data> existing_conversions {};
		std::set<std::pair<utf8string_view, utf8string_view>>
		     existing_aliases {};
	} conversion_data {};
	cnc_conversion_registry_pair_c8_function* on_pairing =
	     [](bool is_alias, size_t from_size, const ztd_char8_t* from_ptr,
	          size_t to_size, const ztd_char8_t* to_ptr, void* user_data_ptr) {
		     user_data_t& user_data = *static_cast<user_data_t*>(user_data_ptr);
		     utf8string_view from(from_ptr, from_size);
		     utf8string_view to(to_ptr, to_size);
		     if (is_alias) {
			     user_data.existing_aliases.insert({ from, to });
		     }
		     else {
			     const encoding_data& from_it
			          = *user_data.existing_conversions.insert({ from }).first;
			     const encoding_data& to_it
			          = *user_data.existing_conversions.insert({ to }).first;
			     const_cast<encoding_data&>(from_it).from = true;
			     const_cast<encoding_data&>(to_it).to     = true;
		     }
	     };
	std::cout << "Available encodings:" << std::endl;
	cnc_registry_pairs_list_c8n(p_registry, on_pairing, &conversion_data);
	if (conversion_data.existing_conversions.empty()) {
		std::cout << "\t (None)" << std::endl;
		return;
	}
	for (const auto& data : conversion_data.existing_conversions) {
		std::cout << "\t" << data.name;
		if (data.from != data.to) {
			if (data.from) {
				std::cout << " [from only]";
			}
			else if (data.to) {
				std::cout << " [to only]";
			}
		}
		std::cout << std::endl;
	}
	for (const auto& data : conversion_data.existing_aliases) {
		std::cout << "\t" << data.first << " " << data.second << " [alias]"
		          << std::endl;
	}
}

void print_help(void) {
	std::cout << "cconv v0.0.0"
	          << R"help(
	format:
		cconv [options]... [input file or -]...

	options:
		-l, --list
		     | list the supported encodings.
		-v, --version
		     | print version.
		    --verbose
		     | show additional informational messages.
		-?, --help
		     | show help.
		-o, --output
		     | output file.
		-s, --silent
		-q, --quiet
		     | do not report any errors to the console, if there are any.
		-f, --from-code (code)
		     | defaut: execution
		       the encoding code "some_code" to convert from, applied to stdin
		       and all input files.
		-t, --to-code some_code
		     | default: utf-8
		       the encoding code "some_code" to convert to.
		-c, --cache positive-integer
		     | default: 1024
		       the number of bytes to default to for the output buffer. Any
		       number below 64 will be clamped to 64.
		
	conversion error handling options:
		-d, --discard-on-failure
		     | default
		       discard any failure and simply leave any unconvertible text
		       out of the input.
		-r, --report-on-failure
		     | stops when a failure occurs and reports an error.
		-b, --byte-substitution data-string
		     | byte-based substition read directly as-is from command line and
		       treated as a sequence of bits to blast into the output.
	
	inputs:
		[-, input file]...
		     | default: -
		       after options, read from stdin (signified by passing nothing, or
		       passing one or more "-") or read an input file, zero or more.)help"
	          << std::endl;
}

void print_version(void) {
	std::cout << "cconv v0.0.0"
	          << "\t Inspired by a request from Tom Honermann about proving "
	             "iconv compatibility."
	          << std::endl
	          << R"license(
	Copyright © JeanHeyd "ThePhD" Meneide and Shepherd's Oasis, LLC
	Contact: opensource@soasis.org

	Apache License Version 2 Usage
	This binary may be used under the terms of Apache License
	Version 2.0 (the "License"); you may not use this file except in compliance
	with the License. You may obtain a copy of the License at

 https://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.)license"
	          << std::endl;
}
