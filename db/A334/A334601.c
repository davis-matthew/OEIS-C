/*
 * A334601  Positive integers m such that sum of cubes of the digits of m,
 *          t=A055012(m), is a multiple of m (m/A055012(m) is an integer >=
 *          1).
 * https://oeis.org/A334601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   sum_of_cubes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A334601 – the n‑th positive integer m such that the sum of the
   cubes of its decimal digits is a multiple of m (t % m == 0). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/sum_of_cubes.h"

/* All solutions lie in the range 1 … 2916, so we can pre‑compute them
   once and answer any query from the table. */
static seq_t seq_sol[2917];
static size_t seq_len = 0;
static bool seq_computed = false;

/* Build the complete table (executed only on first call of a). */
static void build_sequence(void)
{
    for (uint64_t m = 1; m <= 2916; ++m) {
        seq_t t = sum_of_cubes(m);
        if (t % (seq_t)m == 0) {
            seq_sol[seq_len++] = (seq_t)m;
        }
    }
    seq_computed = true;
}

/* External linkage required by the test harness. */
seq_t a(idx_t n)
{
    if (!seq_computed) {
        build_sequence();
    }
    if (n == 0) {
        return (seq_t)0;          /* undefined for n=0 */
    }
    if ((size_t)(n - 1) < seq_len) {
        return seq_sol[(size_t)(n - 1)];
    }
    return (seq_t)0;              /* beyond the last valid term */
}
