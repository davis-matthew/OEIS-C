/*
 * A325191  Number of integer partitions of n such that the difference between
 *          the length of the minimal triangular partition containing and the
 *          maximal triangular partition contained in the Young diagram is 1.
 * https://oeis.org/A325191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Σ_{t≥1, t(t+1)/2 ≤ n} C(t+1, n−t(t+1)/2) with 1 ≤
 *            n−t(t+1)/2 ≤ t
 * Range:     u8 n <= 58, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses the combinatorial identity:
   a(n) = Σ_{t} C(t+1, k)   where k = n−t(t+1)/2 and 1≤k≤t. */

#include "../utils/gcd.h"

static inline void mul128(seq_t a, seq_t b, seq_t *res)
{
#if defined(__SIZEOF_INT128__) && !defined(__cplusplus)
    unsigned __int128 prod = (unsigned __int128)a * b;
    *res = (seq_t)prod;
#else
    *res = a * b;
#endif
}

static inline seq_t binom(int n, int k)
{
    if (k < 0 || k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;      /* symmetry */

    seq_t result = 1;
    for (int i = 1; i <= k; ++i)
    {
        seq_t num = (seq_t)(n - k + i);   /* numerator */
        seq_t den = (seq_t)i;             /* denominator */

        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        g = gcd(result, den);
        result /= g;
        den /= g;

        seq_t prod;
        mul128(result, num, &prod);
        prod /= den;
        result = prod;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t t = 1; ; ++t)
    {
        seq_t B = (seq_t)t * (t + 1) / 2;   /* triangular number t(t+1)/2 */
        if (B > (seq_t)n) break;

        seq_t k = (seq_t)n - B;
        if (k == 0) continue;              /* triangular partition */
        if (k > (seq_t)(t + 1)) continue;  /* offset too large */

        if (k <= (seq_t)t)                  /* 1 ≤ k ≤ t */
        {
            result += binom((int)(t + 1), (int)k);
        }
        /* k == t+1 is excluded (it would be a larger triangular partition) */
    }
    return result;
}
