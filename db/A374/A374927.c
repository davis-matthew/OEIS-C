/*
 * A374927  a(n) = A135318(n)^2.
 * https://oeis.org/A374927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A135318(n)^2. For odd n, a(n) = 4^((n-1)/2). For even n,
 *            let k = n/2. Compute the auxiliary sequence B with B(1)=B(2)=1
 *            and, for i≥3, B(i) = B(i-1) + 2*B(i-2). Then a(2k) = B(k+1)^2.
 *            The function caches B values so that repeated calls are O(1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow4, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow4.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* cache for the auxiliary sequence B */
static seq_t *B_cache = NULL;   /* array of computed B values */
static size_t B_cap   = 0;      /* allocated capacity of B_cache */
static size_t B_len   = 0;      /* number of B values actually stored */

/* Ensure B_cache has B(k+1) available and return it.
   k is non‑negative and the index in B_cache is k (0‑based). */
static seq_t compute_B(idx_t k)
{
    if ((size_t)k >= B_len)
    {
        /* Grow the cache until it can hold index k (i.e. k+1 elements) */
        size_t new_cap = B_cap;
        if (new_cap == 0) new_cap = 2;
        while (new_cap <= (size_t)k + 1)
            new_cap <<= 1;

        seq_t *tmp = seq_realloc(B_cache, B_cap, new_cap);
        B_cache = tmp;
        B_cap   = new_cap;

        /* Compute any new terms */
        for (size_t i = B_len; i <= (size_t)k; ++i)
        {
            if (i < 2)           /* B(1) and B(2) */
                B_cache[i] = (seq_t)1;
            else
                B_cache[i] = B_cache[i - 1] + (B_cache[i - 2] * (seq_t)2);
        }
        B_len = (size_t)k + 1;
    }
    return B_cache[(size_t)k];
}

seq_t a(idx_t n)
{
    if ((n & 1) == 0)          /* even indices */
    {
        idx_t k = n >> 1;      /* n = 2*k */
        seq_t r = compute_B(k);/* r = B(k+1) */
        return r * r;
    }
    else                       /* odd indices */
    {
        idx_t exp = (n - 1) >> 1;   /* (n-1)/2 */
        return pow4(exp);           /* 4^exp */
    }
}
