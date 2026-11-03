/*
 * A256870  Numbers divisible by prime(d+1) for each digit d of their base-10
 *          representation.
 * https://oeis.org/A256870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th nonnegative integer such that each decimal
 *            digit d of a(n) divides the integer by prime(d+1). The sequence
 *            starts with a(1)=0
 * Range:     u8 n <= 9, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
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

/* Cache of already found terms.  */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms stored */
static size_t cache_cap = 0;   /* allocated capacity */
static seq_t next_candidate = 0;

/* Ensure that the cache can hold at least 'needed' elements.  */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed) new_cap <<= 1;
    /* Cast for C++ compatibility.  C allows implicit conversion. */
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* Test whether a given number satisfies the divisibility condition.  */
static bool satisfies(seq_t x)
{
    if (x == 0) return true;          /* 0 has digit 0 → prime(1)=2, and 0 % 2 == 0 */
    seq_t t = x;
    while (t != 0) {
        uint8_t digit = (uint8_t)(t % (seq_t)10);
        seq_t prime = (seq_t)get_prime((idx_t)digit + 1);
        if (x % prime != 0) return false;
        t /= (seq_t)10;
    }
    return true;
}

/* Return the n‑th term (1‑based) of the sequence.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for index 0, but harmless */
    while (cache_len < n) {
        ensure_cache_capacity(cache_len + 1);
        if (satisfies(next_candidate)) {
            cache[cache_len++] = next_candidate;
        }
        next_candidate++;
    }
    return cache[n - 1];
}
