/*
 * A385148  a(n) = A001065(A346878(n)).
 * https://oeis.org/A385148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = σ( σ(2*n) – 2*n ) – ( σ(2*n) – 2*n ) where σ(m) is the
 *            sum of all divisors of m
 * Range:     u8 n <= 44, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   sigma_div
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/sigma_div.h"

seq_t a(idx_t n)
{
    /* offset 1: a(1) is the first element. */
    if (n == 0)
        return 0;

    /* compute m = σ(2*n) – 2*n */
    seq_t two_n = (seq_t)n << 1;            /* 2 * n */
    seq_t sigma_2n = sigma_div(two_n);
    seq_t m = sigma_2n - two_n;

    /* a(n) = σ(m) – m */
    seq_t sigma_m = sigma_div(m);
    return sigma_m - m;
}
