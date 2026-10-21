/*
 * A253588  Upward antidiagonals of array of all multiples of primorial(n),
 *          for each n>0.
 * https://oeis.org/A253588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Primorial(i) * j * The pair (i,j) is obtained by taking
 *            the n‑th position on the * upward antidiagonals of the infinite
 *            array whose rows are * k·Primorial(i), k = 1,2,… . The diagonals
 *            are ordered by * increasing i+j, and within each diagonal the
 *            entries are taken * from larger i to smalle...
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   get_prime, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <limits.h>
#include "../utils/get_prime.h"
#include "../utils/mul_small.h"

static seq_t *prim_cache = NULL;      /* prim_cache[i] = product of first i primes */
static idx_t  prim_cache_len = 0;    /* highest i currently cached (0 means none) */
static idx_t  prim_cache_cap = 0;    /* allocated capacity (including index 0) */

/* Ensure prim_cache contains values up to index 'needed' (inclusive). */
static void prim_cache_ensure(idx_t needed)
{
    if (needed <= prim_cache_len) return;

    /* allocate or grow storage */
    if (prim_cache_cap < needed + 1) { /* +1 for index 0 */
        idx_t new_cap = prim_cache_cap ? prim_cache_cap * 2 : 16;
        while (new_cap < needed + 1) new_cap <<= 1;
        prim_cache = (seq_t *)realloc(prim_cache, new_cap * sizeof(seq_t));
        prim_cache_cap = new_cap;
        if (prim_cache_len == 0)
            prim_cache[0] = (seq_t)1;  /* dummy filler for index 0 */
    }

    /* extend the cache */
    for (idx_t idx = prim_cache_len + 1; idx <= needed; ++idx) {
        seq_t prev = (idx == 1) ? (seq_t)1 : prim_cache[idx - 1];
        seq_t factor = (seq_t)get_prime(idx);   /* first i primes are needed */
        prim_cache[idx] = mul_small(prev, (unsigned int)factor);
    }
    prim_cache_len = needed;
}

/* Integer square root for unsigned __int128 using binary search. */
static idx_t isqrt_u128(unsigned __int128 x)
{
    idx_t low  = 0;
    idx_t high = 1;
    while ((unsigned __int128)high * high <= x)
        high <<= 1;
    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((unsigned __int128)mid * mid <= x)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* Return the a(n) term of OEIS A253588. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined, but safe guard */

    /* Find m such that m*(m-1)/2 >= n with minimal m.
       Compute using integer arithmetic avoiding overflow. */
    unsigned __int128 expr = (unsigned __int128)1 + (unsigned __int128)8 * n;
    idx_t sqrt_val = isqrt_u128(expr);
    idx_t m = (1 + sqrt_val) >> 1;
    while ((unsigned __int128)m * (m - 1) / 2 < n)
        ++m;

    /* Number of terms before diagonal m (i+j = m). */
    unsigned __int128 sb = (unsigned __int128)(m - 2) * (m - 1) / 2;
    idx_t S_before = (idx_t)sb;

    idx_t offset = n - S_before;          /* 1‑based offset within diagonal m */
    idx_t i = m - offset;                 /* row index (primorial index) */
    idx_t j = offset;                     /* column offset (multiplier) */

    /* Ensure primorial for i is cached. */
    prim_cache_ensure(i);

    /* Compute a(n) = Primorial(i) * j. */
    return mul_small(prim_cache[i], (unsigned int)j);
}
