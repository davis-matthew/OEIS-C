/*
 * A360815  Expansion of Sum_{k>=0} x^(3*k) / (1 - k*x)^(k+1).
 * https://oeis.org/A360815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} k^(n-3k) * binomial(n-2k,k)
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* --- local utilities --- */

/* compute binomial(n,k) multiplicatively; works for any seq_t */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    /* use symmetry */
    if (k > n - k)
        k = n - k;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* res = res * (n - k + i) / i;  all in seq_t */
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

/* fast exponentiation for non‑negative integer exponent */
static seq_t seq_pow_uint(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t cur = base;
    while (exp) {
        if (exp & 1)
            result *= cur;
        cur *= cur;
        exp >>= 1;
    }
    return result;
}

/* --- sequence function --- */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3u;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t exp = n - 3u * k;
        seq_t pow_val;

        if (k == 0u) {
            /* 0^0 is defined as 1; otherwise 0 */
            pow_val = (exp == 0u) ? 1 : 0;
        } else {
            pow_val = seq_pow_uint((seq_t)k, exp);
        }

        seq_t bin_val = binom(n - 2u * k, k);
        sum += pow_val * bin_val;
    }
    return sum;
}
