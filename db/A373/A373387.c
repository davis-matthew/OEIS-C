/*
 * A373387  Constant congruence speed of the tetration base n (in radix-10),
 *          or -1 if n is a multiple of 10.
 * https://oeis.org/A373387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   for n ≥ 0 if n % 10 == 0 return -1 (constant speed undefined)
 *            else if n == 0 or n == 1 return 0 else if n % 5 == 1 return v5(n
 *            - 1) else if n % 5 == 2 || n % 5 == 3 return v5(n * n + 1) else
 *            if n % 5 == 4 return max(1, v5(n + 1)) else (n % 5 == 0) return
 *            max(v2(n - 1), v2(n + 1)) where v2(x) coun...
 * Range:     i8 n <= 83, i16 n <= 83, i32 n <= 83, i64 n <= 83, i128 n <= 83,
 *            big any n
 * Helpers:   v5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/v5.h"

/* exponent of 2 in an unsigned integer */
static inline idx_t v2(idx_t x)
{
    idx_t cnt = 0;
    while ((x & 1u) == 0u)
    {
        ++cnt;
        x >>= 1u;
    }
    return cnt;
}

/* helper: max of two idx_t values */
static inline idx_t max_idx_t(idx_t a, idx_t b)
{
    return a > b ? a : b;
}

seq_t a(idx_t n)
{
    /* handle special cases first */
    if (n == 0u || n == 1u)
        return (seq_t)0;

    if (n % 10u == 0u)
        return (seq_t)-1;

    idx_t mod5 = n % 5u;

    if (mod5 == 1u)
    {
        /* n ≡ 1 (mod 5) */
        return (seq_t)v5(n - 1u);
    }
    else if (mod5 == 2u || mod5 == 3u)
    {
        /* n ≡ 2 or 3 (mod 5) */
        idx_t sq = n * n + 1u;      /* fits in 64‑bit for all n considered */
        return (seq_t)v5(sq);
    }
    else if (mod5 == 4u)
    {
        /* n ≡ 4 (mod 5) */
        idx_t val = v5(n + 1u);
        return (seq_t)(val ? val : 1u);
    }
    else /* mod5 == 0 (n divisible by 5 but not by 10) */
    {
        return (seq_t)max_idx_t(v2(n - 1u), v2(n + 1u));
    }
}
