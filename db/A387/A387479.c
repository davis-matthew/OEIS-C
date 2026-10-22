/*
 * A387479  a(n) = Sum_{k=0..floor(n/3)} 2^k * 3^(n-3*k) *
 *          binomial(k,n-3*k)^2.
 * https://oeis.org/A387479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/3⌋} 2^k * 3^(n-3k) * binomial(k, n-3k)^2
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 33, u64 n <= 63, u128 n <= 122,
 *            big any n
 * Helpers:   binomial, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/pow3.h"

/* Compute the OEIS sequence A387479. */
seq_t a(idx_t n)
{
    idx_t k_max = n / 3;          /* largest k with n-3k >= 0 */
    seq_t sum = 0;

    for (idx_t k = 0; k <= k_max; ++k) {
        idx_t m = n - 3 * k;      /* n-3k, number of 3's */
        if (m > k) continue;     /* binomial(k, m) would be zero; guard */

        seq_t b = binomial(k, m);
        if (b == 0) continue;    /* skip zero terms */

        seq_t term = b * b;       /* binomial^2 */
        term = term * pow2(k);    /* *2^k */
        term = term * pow3(m);    /* *3^(n-3k) */

        sum = sum + term;
    }

    return sum;
}
