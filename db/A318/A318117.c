/*
 * A318117  Number of compositions of n into exactly n nonnegative parts <=
 *          ten.
 * https://oeis.org/A318117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = [x^n] ((x^11-1)/(x-1))^n
 *                = sum_{k=0..floor(n/11)} (-1)^k * C(n,k) * C(2*n-11*k-1, n-11*k)
 */

#include <stdint.h>

#ifdef __cplusplus
#include <type_traits>
#endif

/* integer binomial using 128‑bit intermediate to avoid overflow */
static inline seq_t binomial_integral(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = (seq_t)((__uint128_t)res * (n - k + i) / i);
    }
    return res;
}

/* generic binomial using only integer arithmetic; safe for arbitrary‑precision seq_t */
static inline seq_t binomial_bigint(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t numer = (seq_t)(n - k + i);
        seq_t denom = (seq_t)i;
        res = (res * numer) / denom;   /* exact division */
    }
    return res;
}

/* select the proper binomial implementation */
static inline seq_t binomial(idx_t n, idx_t k)
{
#ifdef __cplusplus
    if constexpr (std::is_integral<seq_t>::value)
        return binomial_integral(n, k);
    else
        return binomial_bigint(n, k);
#else
    return binomial_integral(n, k);
#endif
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t result = 0;
    idx_t maxk = n / 11;

    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t term1 = binomial(n, k);
        idx_t top  = 2 * n - 11 * k - 1;
        idx_t bot  = n - 11 * k;
        seq_t term2 = binomial(top, bot);
        seq_t term = term1 * term2;

        if (k & 1)
            result -= term;
        else
            result += term;
    }

    return result;
}
