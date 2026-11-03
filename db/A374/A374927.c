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
 * Validated: standard, pass, 51 terms through n = 915 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

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
