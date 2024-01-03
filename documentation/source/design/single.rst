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

Singular Conversion
===================

Singular conversions are foundational because they are guaranteed to only encode a single complete unit of information and output a single complete unit of information (or change the operating state in some manner), per doing :doc:`one indivisible unit of work </design/indivisible>`.

Singular conversions also have a maximally-bounded input size that is guaranteed to either change the state or compute one complete unit of output (which may result in multiple bytes/code units/code points being written out). This means that one can, for each individual operation, provide enough space to guarantee that doing an output operation will never be more than a statically-known, maximally-sized buffer (and thus avoid ever having to handle the :cpp:enumerator:`cnc_mcerr_insufficient_output` return code). This is necessary for low-memory environments which may need to process input with the smallest possible memory guarantee available. As an example, given the general form of a function (whose parameters and style is discussed in the :doc:`functions section </design/function>`), one can put together a concrete general-purpose 

Looping over a set of input using a singular conversion is a valid way to transform a singular conversion into a bulk conversion. This technique is handy for guaranteeing correctness from composing a bulk operation from a correctly-implemented single conversion, but hinders throughput and speed due to needing to check for safety and constantly recalculate and update sizes. This does not necessarily mean that singular conversion must always be done: there are techniques that can convert multiple indivisible units of input at a time through much more efficient means.

The benefit of a Single Conversion API that only performs one indivisible unit of work is the ability to build every single other piece of the API through this very functionality.

conversion algorithms to become single conversion algorithms, though the performance of such an algorithm is noticeably worse.
