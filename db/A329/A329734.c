/*
 * A329734  a(n) = (10^(1 + floor((n-1) * log_10 2)) + 1)^n, n >= 0.
 * https://oeis.org/A329734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (10^(1 + floor((n-1) * log10 2)) + 1)^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   big_pow, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use the supplied utilities pow10 and big_pow. */
#include "../utils/pow10.h"
#include "../utils/big_pow.h"

#define LOG10_2_NUM  301029996ULL   /* floor(log10(2) * 1e9) */
#define LOG10_2_DEN  1000000000ULL  /* scaling factor */

seq_t a(idx_t n)
{
    /* a(0) is defined as 1. */
    if (n == 0u) {
        return (seq_t)1;
    }

    /* Use integer arithmetic to compute d(n) = 1 + floor((n-1)*log10(2)) */
    uint64_t kk   = n - 1ULL;                      /* k = n-1  (k >= 0)   */
    uint64_t prod = kk * LOG10_2_NUM;              /* product fits in 64bit for n ≤ 10000 */
    uint64_t d    = prod / LOG10_2_DEN + 1ULL;     /* number of decimal digits of 2^(n-1) */

    /* Compute base = 10^d + 1 and then (base)^n */
    seq_t base = pow10(d);            /* 10^d as seq_t */
    base = base + (seq_t)1;           /* add 1 */

    seq_t result = big_pow(base, (seq_t)n);  /* (10^d + 1)^n */

    return result;
}
