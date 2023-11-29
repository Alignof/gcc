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

// clang-format off
#include "config.h"
#define INCLUDE_STRING
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "target.h"
#include "rtl.h"
#include "tree.h"
#include "memmodel.h"
#include "gimple.h"
#include "tm_p.h"
#include "stringpool.h"
#include "attribs.h"
#include "emit-rtl.h"
#include "gimplify.h"
#include "gimple-iterator.h"
#include "dumpfile.h"
#include "rtl-iter.h"
#include "cfgrtl.h"
#include "tree-pass.h"
#include "cgraph.h"
// clang-format on

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

static unsigned int insert_lpad(void) {
  rtx lpad_insn;
  rtx_insn *insn;
  basic_block bb;

  FOR_EACH_BB_FN(bb, cfun) {
    for (insn = BB_HEAD(bb); insn != NEXT_INSN(BB_END(bb));
         insn = NEXT_INSN(insn)) {

      // non-local goto
      if (LABEL_P(insn) &&
          (LABEL_PRESERVE_P(insn) || bb->flags & BB_NON_LOCAL_GOTO_TARGET)) {
        lpad_insn = riscv_gen_lpad();
        emit_insn_after(lpad_insn, insn);
        continue;
      }

      /* There could still be more labels that are valid targets of a BTI J
       * instuction. To find them we start looking through the JUMP_INSN. If it
       * jumps to a jump table, then we find all labels of the jump table to
       * protect with a BTI J.
       */
      if (JUMP_P(insn)) {
        rtx_jump_table_data *table;
        if (tablejump_p(insn, NULL, &table)) {
          rtvec vec = table->get_labels();
          int j;
          rtx_insn *label;

          for (j = GET_NUM_ELEM(vec) - 1; j >= 0; --j) {
            label = as_a<rtx_insn *>(XEXP(RTVEC_ELT(vec, j), 0));
            rtx_insn *next = next_nonnote_nondebug_insn(label);
            if (riscv_lpad_insn_p(next))
              continue;

            lpad_insn = riscv_gen_lpad();
            emit_insn_after(lpad_insn, label);
          }
        }
      }
    }
  }

  if (!cgraph_node::get(cfun->decl)->only_called_directly_p()) {
    bb = ENTRY_BLOCK_PTR_FOR_FN(cfun)->next_bb;
    insn = BB_HEAD(bb);
    lpad_insn = riscv_gen_lpad();
    emit_insn_before(lpad_insn, insn);
  }

  return 0;
}

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
