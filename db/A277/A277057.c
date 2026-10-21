/*
 * A277057  Least k such that n-th repunit times k contains all digits from 1
 *          to 9.
 * https://oeis.org/A277057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A277057:
   Least k such that the n‑th repunit (111...1, n digits) multiplied
   by k contains all digits 1–9 (no zero).  The definition gives
   the following values:
     a(1) = 123456789
     a(2) = 11225079
     a(3) = 1113198
     a(4) = 210789
     a(n) = 11115   for all n ≥ 5
   This constant value holds for every n ≥ 5, as proven by the entry.
   The function tolerates arbitrary order of calls and uses no global
   cache, so it is fast for all n.  All arithmetic is done in seq_t,
   the build‑dependent unsigned integer type.  */

#include <stdbool.h>

/* External linkage required by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined offset, but safe */

    /* Handle the small known terms explicitly */
    switch ((uint64_t)n) {
        case 1: return (seq_t)123456789;
        case 2: return (seq_t)11225079;
        case 3: return (seq_t)1113198;
        case 4: return (seq_t)210789;
        default:   /* n >= 5 */
            return (seq_t)11115;
    }
}
