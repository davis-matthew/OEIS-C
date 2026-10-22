/*
 * A373470  First differences of A043555 = number of runs in base-3
 *          representation of n.
 * https://oeis.org/A373470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, less
 * Formula:   a(n) = A043555(n+1) – A043555(n) where A043555(n) is the number
 *            of runs in the base‑3 representation of n. The runs of 0 are
 *            defined as one
 * Range:     i8 n <= 99, i16 n <= 99, i32 n <= 99, i64 n <= 99, i128 n <= 99,
 *            big any n
 * Helpers:   base3_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/base3_runs.h"

/* a(n) = first differences of A043555, i.e. runs(n+1) - runs(n). */
seq_t a(idx_t n)
{
    seq_t r_n   = base3_runs(n);      /* runs for n  */
    seq_t r_np1 = base3_runs(n + 1);  /* runs for n+1 */
    return r_np1 - r_n;
}
