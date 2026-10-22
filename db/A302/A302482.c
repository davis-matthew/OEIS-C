/*
 * A302482  Elementary automaton rules that include every possible substring
 *          of length 3 in an output.
 * https://oeis.org/A302482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 162, u16 n <= 162, u32 n <= 162, u64 n <= 162,
 *            u128 n <= 162, big any n
 * Helpers:   rule_is_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A302482: elementary automaton rules that include every possible substring
   of length 3 in an output.  The routine enumerates all 256 ECA rules,
   retains those whose 5‑bit input windows generate all 8 possible
   3‑bit output triples, and returns the nth such rule (1‑based). */

#include <stddef.h>   /* for size_t */
#include "../utils/rule_is_valid.h"

static seq_t rule_list[256];
static size_t rule_count = 0;

/* Build the list of valid rules (performed on first call). */
static void build_rule_list(void) {
    for (idx_t r = 0; r <= 255; ++r) {
        if (rule_is_valid(r))
            rule_list[rule_count++] = (seq_t)r;
    }
}

seq_t a(idx_t n) {
    if (rule_count == 0)
        build_rule_list();

    if (n >= 1 && n <= rule_count)
        return rule_list[n - 1];
    return 0;   /* undefined for n > rule_count; 0 is safe sentinel */
}
