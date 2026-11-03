/*
 * A325277  Irregular triangle read by rows where row 1 is {1} and row n is
 *          the sequence starting with n and repeatedly applying A181819 until
 *          a prime number is reached.
 * https://oeis.org/A325277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-03)
 * Helpers:   factor_u64, is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A325277
   Concatenation of the chains of A181819(n) starting at each integer n
   and ending when a prime is reached.  That is, the triangle is
   triangle(n, k) = the k-th element of the iteration starting with n
   (where the first element is n itself).  The sequence a(n) is
   obtained by reading this triangle row by row.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"
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

/*---------------------------------
  Helper:  A181819(n)  -- mapping function
  ---------------------------------*/
static seq_t a181819_value(seq_t n)
{
    if (n < 2)                /* 1 is treated as a fixed point */
        return 1;

    uint64_t val = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(val, primes, exps, &cnt);

    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = get_prime((idx_t)exps[i]);     /* exponent → prime */
        res *= p;
    }
    return res;
}

/*---------------------------------
  Dynamic array for the flattened triangle
  ---------------------------------*/
static seq_t *cache = NULL;   /* storage for a(1), a(2), ... */
static size_t cache_cap = 0;  /* allocated capacity */
static size_t cache_len = 0;  /* number of elements stored */

/* ensure the cache can hold at least 'need' elements */
static void ensure_cache_capacity(size_t need)
{
    if (cache_len >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (tmp == NULL) {
        /* Allocation failure – abort (since this is a library function) */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* append a value to the cache (expanding if necessary) */
static void append_to_cache(seq_t val)
{
    ensure_cache_capacity(cache_len + 1);
    cache[cache_len++] = val;
}

/*---------------------------------
  Main function
  ---------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) {
        /* a(0) is undefined by the OEIS offset; return 0 as placeholder */
        return (seq_t)0;
    }

    /* Generate rows until we have at least n elements in the cache */
    static idx_t next_row = 1;          /* next row number to generate */

    while (cache_len < (size_t)n) {
        if (next_row == 1) {
            append_to_cache((seq_t)1);  /* row 1 is {1} */
        } else {
            seq_t val = (seq_t)next_row;
            while (true) {
                append_to_cache(val);
                if (is_prime((idx_t)val))
                    break;
                val = a181819_value(val);
            }
        }
        ++next_row;
    }

    return cache[n - 1];
}
