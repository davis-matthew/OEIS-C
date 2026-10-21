/*
 * A261680  Number of ordered quadruples (u,v,w,x) of binary palindromes (see
 *          A006995) with u+v+w+x=n.
 * https://oeis.org/A261680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 3203, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   palindrome_from_half
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: count ordered quadruples (u,v,w,x) of binary palindromes
   such that u + v + w + x = n.  Enumerates all binary palindromes ≤ n,
   stores their pairwise sums, and convolves the pair‑sum array with itself
   to obtain the quadruple count. */
#include <stdlib.h>
#include "../utils/palindrome_from_half.h"

/* Return the position of the most significant bit of n (0‑based)
   or 0 if n==0. */
static idx_t msb_index(idx_t n)
{
    idx_t r = 0;
    while (n >>= 1)
        ++r;
    return r;
}

/* Generate all binary palindromes ≤ n, including 0, store them in *out
   and set *cnt to the number of palindromes.  Out must be freed by caller. */
static void generate_palindromes(idx_t n, idx_t **out, size_t *cnt)
{
    size_t cap = 4;
    idx_t *pal = (idx_t *)malloc(cap * sizeof(idx_t));
    size_t sz = 0;

    /* 0 is a binary palindrome */
    if (n >= 0)
    {
        pal[sz++] = 0;
    }

    /* Find maximum bit length needed */
    idx_t max_len = msb_index(n) + 1;

    for (idx_t len = 1; len <= max_len; ++len)
    {
        idx_t half_bits = (len + 1) / 2;
        idx_t start = (idx_t)1 << (half_bits - 1);     /* leading 1 */
        idx_t end   = (idx_t)1 << half_bits;           /* exclusive */

        for (idx_t half = start; half < end; ++half)
        {
            idx_t p = palindrome_from_half(half, len);
            if (p > n)
                continue;

            if (sz == cap)
            {
                cap <<= 1;
                idx_t *tmp = (idx_t *)realloc(pal, cap * sizeof(idx_t));
                if (!tmp)
                {
                    free(pal);
                    *out = NULL;
                    *cnt = 0;
                    return;
                }
                pal = tmp;
            }
            pal[sz++] = p;
        }
    }

    *out = pal;
    *cnt = sz;
}

seq_t a(idx_t n)
{
    /* Special case n == 0 */
    if (n == 0)
        return (seq_t)1;

    idx_t *pal = NULL;
    size_t pal_cnt = 0;
    generate_palindromes(n, &pal, &pal_cnt);
    if (!pal)
        return (seq_t)0;          /* allocation failure */

    /* Allocate pair-sum array */
    size_t arr_size = (size_t)n + 1;
    seq_t *pair_sum = (seq_t *)calloc(arr_size, sizeof(seq_t));
    if (!pair_sum)
    {
        free(pal);
        return (seq_t)0;          /* allocation failure */
    }

    /* Count ordered pairs (u,v) with sum <= n */
    for (size_t i = 0; i < pal_cnt; ++i)
    {
        idx_t u = pal[i];
        for (size_t j = 0; j < pal_cnt; ++j)
        {
            idx_t v   = pal[j];
            idx_t s   = u + v;
            if (s <= n)
                pair_sum[(size_t)s] += (seq_t)1;
        }
    }

    /* Convolution of pair sums with itself */
    seq_t ans = (seq_t)0;
    for (idx_t s = 0; s <= n; ++s)
    {
        idx_t t = n - s;
        ans += pair_sum[(size_t)s] * pair_sum[(size_t)t];
    }

    free(pair_sum);
    free(pal);

    return ans;
}
