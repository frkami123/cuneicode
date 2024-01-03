.. ============================================================================
..
.. ztd.cuneicode
.. Copyright © JeanHeyd "ThePhD" Meneide and Shepherd's Oasis, LLC
.. Contact: opensource@soasis.org
..
.. Commercial License Usage
.. Licensees holding valid commercial ztd.cuneicode licenses may use this file in
.. accordance with the commercial license agreement provided with the
.. Software or, alternatively, in accordance with the terms contained in
.. a written agreement between you and Shepherd's Oasis, LLC.
.. For licensing terms and conditions see your agreement. For
.. further information contact opensource@soasis.org.
..
.. Apache License Version 2 Usage
.. Alternatively, this file may be used under the terms of Apache License
.. Version 2.0 (the "License") for non-commercial use; you may not use this
.. file except in compliance with the License. You may obtain a copy of the
.. License at
..
.. https://www.apache.org/licenses/LICENSE-2.0
..
.. Unless required by applicable law or agreed to in writing, software
.. distributed under the License is distributed on an "AS IS" BASIS,
.. WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
.. See the License for the specific language governing permissions and
.. limitations under the License.
..
.. ========================================================================= ..

``ztd.cuneicode``
=================

The premiere library for handling text in different encoding forms for C software.



Who Is This Library For?
------------------------

If:

- you want to convert from one Unicode encoding to another Unicode encoding;
- you want to prevent data in the wrong encoding from infiltrating your application and causing `Mojibake <https://en.wikipedia.org/wiki/Mojibake>`_;
- you want to have a flexible, growable registry of encodings that does not require compile-time modification of source code to expand;
- you want a design that is better than the C Standard Library's and actually handles your encoding;
- you want safe defaults for working with text;

then ``ztd.cuneicode`` is for you!

.. toctree::
	:maxdepth: 1
	:caption: Contents:

	quick
	design
	api
	definitions
	known Unicode encodings
	future
	benchmarks
	license
	bibliography




Indices & Search
================

.. toctree::

	genindex
