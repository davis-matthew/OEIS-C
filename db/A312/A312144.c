/*
 * A312144  Coordination sequence Gal.6.151.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coordination sequence for Gal.6.151.1.
   The sequence satisfies:
   a(0) = 1, a(1) = 4, a(2) = 8,
   for n ≥ 3   a(n) = a(n-1) + d(n)
   where d(n) = 5 if (n % 12) is in [3,10] else 4.
   This can be implemented in O(1) time per query. */
#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)8;

    /* Steps from i = 3 up to n inclusive. */
    idx_t steps = n - 2;                /* number of increments needed */
    idx_t full_cycles = steps / 12;     /* complete 12‑term blocks */
    idx_t rem = steps % 12;             /* remaining terms */

    /* Sum of one full cycle: eight 5’s and four 4’s = 56 */
    seq_t result = (seq_t)8 + (seq_t)full_cycles * (seq_t)56;

    /* Process the remaining terms (at most 11). */
    for (idx_t k = 0; k < rem; ++k) {
        /* Current index is i = 3 + k */
        idx_t mod = (3 + k) % 12;
        seq_t add = (mod >= 3 && mod <= 10) ? (seq_t)5 : (seq_t)4;
        result += add;
    }
    return result;
}
