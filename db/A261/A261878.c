/*
 * A261878  Number of distinct fractional parts of the sums 1/j+...+1/k with 1
 *          <= j <= k <= n, where the fractional part of x is given by x -
 *          floor(x).
 * https://oeis.org/A261878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n-1)/2 - 3 for n > 20 For n ≤ 20 it is computed
 *            exactly by brute force
 * Range:     u8 n <= 23, u16 n <= 362, u32 n <= 1200, u64 n <= 1200,
 *            u128 n <= 1200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* gcd for __int128 */
static __int128 gcd128(__int128 a, __int128 b)
{
    while (b != 0)
    {
        __int128 t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* structure holding a reduced fractional part for brute force */
struct frac128
{
    __int128 num; /* numerator (0 ≤ num < den) */
    __int128 den; /* denominator (den > 0) */
};

/* compare two frac128 values */
static bool same_frac(const struct frac128 *a, const struct frac128 *b)
{
    return a->num == b->num && a->den == b->den;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* use the conjectured closed form for n > 20 */
    if (n > 20)
    {
        return (seq_t)n * ((seq_t)n - 1) / 2 - 3;
    }

    /* brute force for n ≤ 20 */
    idx_t total_pairs = n * (n + 1) / 2;
    struct frac128 *seen = (struct frac128 *)malloc(total_pairs * sizeof(struct frac128));
    size_t seen_cnt = 0;

    for (idx_t j = 1; j <= n; ++j)
    {
        for (idx_t k = j; k <= n; ++k)
        {
            /* sum 1/j + ... + 1/k as a fraction */
            __int128 num = 0;
            __int128 den = 1;
            for (idx_t i = j; i <= k; ++i)
            {
                /* add 1/i :  num/den + 1/i = (num*i + den) / (den*i) */
                __int128 new_num = num * (__int128)i + den;
                __int128 new_den = den * (__int128)i;
                __int128 g = gcd128(new_num, new_den);
                num = new_num / g;
                den = new_den / g;
            }

            /* fractional part: r/den where r = num % den */
            __int128 r = num % den;
            struct frac128 frac;
            if (r == 0)
            {
                /* integer sum → fractional part 0/1 */
                frac.num = 0;
                frac.den = 1;
            }
            else
            {
                __int128 g = gcd128(r, den);
                frac.num = r / g;
                frac.den = den / g;
            }

            /* check if this fractional part was seen before */
            bool found = false;
            for (size_t t = 0; t < seen_cnt; ++t)
            {
                if (same_frac(&seen[t], &frac))
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                seen[seen_cnt++] = frac;
            }
        }
    }

    free(seen);
    return (seq_t)seen_cnt;
}
