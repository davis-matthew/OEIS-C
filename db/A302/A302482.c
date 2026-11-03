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
 * Validated: full, pass, 162 terms through n = 162 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A302482: elementary automaton rules that include every possible substring
   of length 3 in an output.  The routine enumerates all 256 ECA rules,
   retains those whose 5‑bit input windows generate all 8 possible
   3‑bit output triples, and returns the nth such rule (1‑based). */

#include <stddef.h>   /* for size_t */

/* Helper: compute whether a rule is valid. */
static int rule_is_valid(idx_t rule) {
    unsigned mask = 0;
    for (int p = 0; p < 32; ++p) {           /* all 5‑bit patterns */
        unsigned a = (p >> 4) & 1U;
        unsigned b = (p >> 3) & 1U;
        unsigned c = (p >> 2) & 1U;
        unsigned d = (p >> 1) & 1U;
        unsigned e = (p >> 0) & 1U;

        unsigned idx1 = (a << 2) | (b << 1) | c;   /* 111..000 order */
        unsigned idx2 = (b << 2) | (c << 1) | d;
        unsigned idx3 = (c << 2) | (d << 1) | e;

        unsigned o1 = (rule >> idx1) & 1U;
        unsigned o2 = (rule >> idx2) & 1U;
        unsigned o3 = (rule >> idx3) & 1U;

        unsigned out = (o1 << 2) | (o2 << 1) | o3;
        mask |= 1u << out;
    }
    return mask == 0xFFu;              /* all 8 3‑bit triples present */
}

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
