/* Branch Target Identification for AArch64 architecture.
   Copyright (C) 2019-2023 Free Software Foundation, Inc.
   Contributed by Norimasa Takana

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GCC is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.
*/

// anon namespace
namespace {
class pass_insert_lpad : public rtl_opt_pass {
public:
  pass_insert_lpad(gcc::context *ctxt)
      : rtl_opt_pass(pass_data_insert_lpad, ctxt) {}

}; // class pass_insert_lpad
} // namespace

rtl_opt_pass *make_pass_insert_lpad(gcc::context *ctxt) {
  return new pass_insert_lpad(ctxt);
}
