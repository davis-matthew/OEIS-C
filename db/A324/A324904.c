/*
 * A324904  The 2-adic valuation of A318466(n), where A318466(n) = 2*n OR
 *          sigma(n).
 * https://oeis.org/A324904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = v₂( (2*n) | σ(n) ) σ(n) is the sum of divisors of n. a(n)
 *            is the 2‑adic valuation of the bitwise OR of 2*n and σ(n)
 * Range:     u8 n <= 9994, u16 n <= 9994, u32 n <= 9994, u64 n <= 9994,
 *            u128 n <= 9994, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* not used, but keeps the function safe if called with 0 */
        return (seq_t)0;

    seq_t sigma = sigma_of(n);                    /* compute σ(n) */
    seq_t x     = ((seq_t)n << 1) | sigma;        /* 2*n OR σ(n) */

    seq_t val = 0;                                 /* 2-adic valuation */
    while ((x & (seq_t)1) == 0) {                 /* while lowest bit is 0 */
        x >>= 1;
        ++val;
    }

    return val;
}
