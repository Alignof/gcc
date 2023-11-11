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

#define IN_TARGET_CODE 1

#include "config.h"
#define INCLUDE_STRING
#include "attribs.h"
#include "backend.h"
#include "cfgrtl.h"
#include "cgraph.h"
#include "coretypes.h"
#include "dumpfile.h"
#include "emit-rtl.h"
#include "gimple-iterator.h"
#include "gimple.h"
#include "gimplify.h"
#include "memmodel.h"
#include "rtl-iter.h"
#include "rtl.h"
#include "stringpool.h"
#include "system.h"
#include "target.h"
#include "tm_p.h"
#include "tree-pass.h"
#include "tree.h"

// anon namespace
namespace {

const pass_data pass_data_insert_lpad = {
    RTL_PASS,      /* type.  */
    "lpad",        /* name.  */
    OPTGROUP_NONE, /* optinfo_flags.  */
    TV_MACH_DEP,   /* tv_id.  */
    0,             /* properties_required.  */
    0,             /* properties_provided.  */
    0,             /* properties_destroyed.  */
    0,             /* todo_flags_start.  */
    0,             /* todo_flags_finish.  */
};

static unsigned int insert_lpad(void) { return 0; }

class pass_insert_lpad : public rtl_opt_pass {
public:
  pass_insert_lpad(gcc::context *ctxt)
      : rtl_opt_pass(pass_data_insert_lpad, ctxt) {}

  virtual bool gate(function *) {
    // return zicfilp_enabled();
    return true;
  }

  virtual unsigned int execute(function *) { return insert_lpad(); }

}; // class pass_insert_lpad
} // namespace

rtl_opt_pass *make_pass_insert_lpad(gcc::context *ctxt) {
  return new pass_insert_lpad(ctxt);
}
