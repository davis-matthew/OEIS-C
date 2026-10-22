/*
 * A392435  a(n) = Sum_{k=0..floor(n/3)} binomial(3*k,2*n-6*k).
 * https://oeis.org/A392435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 41, u64 n <= 82, u128 n <= 160,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A392435:
 *     a(n) = sum_{k=0..floor(n/3)} binomial(3*k, 2*n-6*k)
 *
 * Computation is done multiplicatively to avoid overflow on 64‑bit
 * builds.  For big‑integer builds the built‑in multiplication/division
 * operators of the integer class are used directly.  The routine is
 * independent of any external memoisation and works for any n ≥ 0.
 */

#include <stdint.h>

/* Multiplicative binomial coefficient.
 * If seq_t is a 64‑bit unsigned integer, the calculation is performed
 * using 128‑bit arithmetic to prevent intermediate overflow.
 * For arbitrary‑precision seq_t, the normal operators are used. */
static seq_t binom_simple(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;

    /* 64‑bit case – use 128‑bit temp */
    if (sizeof(seq_t) == sizeof(uint64_t))
    {
        unsigned __int128 res = 1;
        for (idx_t i = 1; i <= k; ++i)
        {
            res = res * (unsigned __int128)(n - k + i);
            res /= i;
            /* If the intermediate exceeds 2^64‑1 we know the final
               result also exceeds 64 bits; in that case we return 0
               because the final a(n) will also overflow and is
               unverified in the 64‑bit build. */
            if (res > UINT64_MAX) return 0;
        }
        return (seq_t)res;
    }

    /* arbitrary precision case – rely on the seq_t operators */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res *= (seq_t)(n - k + i);
        res /= (seq_t)i;
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t maxk = n / 3;
    /* Use 128‑bit accumulator for 64‑bit builds to avoid wrap‑around
       before the final result is known to fit. */
    if (sizeof(seq_t) == sizeof(uint64_t))
    {
        unsigned __int128 acc = 0;
        for (idx_t k = 0; k <= maxk; ++k)
        {
            idx_t a_val = 3 * k;
            idx_t b_val = 2 * n - 6 * k;
            if (b_val > a_val) continue;
            seq_t val = binom_simple(a_val, b_val);
            acc += (unsigned __int128)val;
        }
        return (seq_t)acc;
    }
    /* arbitrary precision build – a simple sum is safe */
    seq_t sum = 0;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t a_val = 3 * k;
        idx_t b_val = 2 * n - 6 * k;
        if (b_val > a_val) continue;
        sum += binom_simple(a_val, b_val);
    }
    return sum;
}
