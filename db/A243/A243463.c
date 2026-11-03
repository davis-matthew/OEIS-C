/*
 * A243463  Numbers n such that prime(n) and phi(n) have no common digits.
 * https://oeis.org/A243463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 118, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Validated: standard, pass, 80 terms through n = 2825 (2026-11-03)
 * Helpers:   mem, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: a(n) is the n‑th integer k such that prime(k) and phi(k) share no digits. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/totient.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

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

/* helper: bit mask of decimal digits contained in a 64‑bit integer */
static inline uint16_t digit_mask_uint64(uint64_t x)
{
    uint16_t mask = 0;
    do {
        mask |= 1u << (x % 10);
        x /= 10;
    } while (x);
    return mask;
}

static seq_t *cache = NULL;        /* cached terms of the sequence   */
static size_t cache_len = 0;       /* number of cached terms         */
static size_t cache_cap = 0;       /* current capacity of 'cache'    */
static uint64_t tested_up_to = 0;  /* next candidate n to test (1‑based) */

/* ensure that 'cache' contains at least 'target' terms */
static void ensure_to(idx_t target)
{
    if ((size_t)target <= cache_len)
        return;                     /* already have enough terms */

    if (!cache)                     /* allocate initial buffer   */
        grow_seq(&cache, &cache_cap, (size_t)target);

    while (cache_len < (size_t)target)
    {
        uint64_t n = ++tested_up_to;       /* candidate integer             */
        seq_t  phi_n  = totient(n);
        uint64_t phi_val = (uint64_t)phi_n;  /* safe: phi(n) ≤ n ≤ 2^64‑1     */
        uint32_t prime_val = get_prime(n);   /* nth prime, 1‑indexed            */

        uint16_t mask_p = digit_mask_uint64((uint64_t)prime_val);
        uint16_t mask_t = digit_mask_uint64(phi_val);

        if ((mask_p & mask_t) == 0)  /* no common decimal digit */
        {
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = (seq_t)n;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        abort();                     /* sequence offset is 1            */
    ensure_to(n);
    return cache[n - 1];
}
