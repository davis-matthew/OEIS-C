/*
 * A361853  Number of integer partitions of n such that (length) * (maximum) =
 *          2n.
 * https://oeis.org/A361853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts partitions of n such that (length)*(maximum)=2n
 * Range:     u8 n <= 29, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   count_exact_k_max_eq_m
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Needed standard headers */
#include <stdlib.h>
#include <string.h>
#include "../utils/count_exact_k_max_eq_m.h"

/* ------------------------------------------------------------------
   Main entry: a(n)
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    seq_t result = 0;
    idx_t two_n = 2 * n;

    /* enumerate all factor pairs (k, m) of 2n */
    for (idx_t d = 1; d * d <= two_n; ++d) {
        if (two_n % d != 0) continue;
        idx_t k1 = d;          /* length */
        idx_t m1 = two_n / d;  /* maximum part */

        /* first pair (k1, m1) */
        if (k1 <= n && m1 <= n) {
            result += count_exact_k_max_eq_m(n, k1, m1);
        }

        /* second pair (k2, m2), unless d*d == two_n */
        if (d * d == two_n) continue;
        idx_t k2 = two_n / d;
        idx_t m2 = d;
        if (k2 <= n && m2 <= n) {
            result += count_exact_k_max_eq_m(n, k2, m2);
        }
    }

    return result;
}
