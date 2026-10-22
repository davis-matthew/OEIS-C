/*
 * A399976  Number of sublattices of index n in generic n-dimensional lattice.
 * https://oeis.org/A399976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of sublattices of index n in a generic
 *            n-dimensional lattice. The value is computed as the product over
 *            prime powers p^e | n of a_n(p^e), where a_n(p^e) is the
 *            coefficient of t^e in ∏_{i=0}^{n-1} (1 - p^i t)^(-1). This is
 *            evaluated with a simple dynamic program that runs in O(n·e²)...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_int.h"

static seq_t prime_contrib(idx_t dim, uint64_t p, idx_t e)
{
    /* arrays are sized large enough for the maximum exponent that
       appears for n up to 10 000 (log₂(10 000) ≈ 14). */
    seq_t dp[20], newdp[20];
    for (idx_t i = 0; i <= e; ++i) dp[i] = 0;
    dp[0] = 1;

    seq_t base = 1;                      /* base = p^{dim-1} for current iteration */
    for (idx_t d = 1; d <= dim; ++d) {
        newdp[0] = 1;
        for (idx_t k = 1; k <= e; ++k) {
            seq_t sum = 0;
            seq_t power = 1;
            for (idx_t i = 0; i <= k; ++i) {
                sum += power * dp[k - i];
                power *= base;
            }
            newdp[k] = sum;
        }
        /* copy newdp into dp for next dimension */
        for (idx_t i = 0; i <= e; ++i) dp[i] = newdp[i];
        base *= (seq_t)p;   /* advance p^{dim} for next outer loop */
    }
    return dp[e];
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1, but guard just in case */

    seq_t result = 1;
    uint64_t temp = (uint64_t)n;

    /* simple trial division factorisation */
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            seq_t contrib = prime_contrib((idx_t)n, p, exp);
            result *= contrib;
        }
    }
    if (temp > 1) {          /* remaining prime factor  */
        seq_t contrib = prime_contrib((idx_t)n, temp, 1);
        result *= contrib;
    }

    return result;
}
