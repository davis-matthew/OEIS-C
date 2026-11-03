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
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

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
        prim_cache = mem_resize(seq_t, prim_cache, new_cap);
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
