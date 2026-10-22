/*
 * A306292  Number of asymmetric Dyck paths of semilength n.
 * https://oeis.org/A306292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(n) – binom(n, floor(n/2)) = (2n)!/(n!(n+1)!) – n!
 *            / (⌊n/2⌋!·⌈n/2⌉!) (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)    /* not defined for n < 1 – return 0 safely */
        return 0;

    /* Catalan(n) = binom(2n, n) / (n+1) */
    seq_t cat = binomial(2 * n, n) / (n + 1);

    /* binom(n, floor(n/2)) – integer division is exact */
    seq_t mid = binomial(n, n / 2);

    return cat - mid;
}
