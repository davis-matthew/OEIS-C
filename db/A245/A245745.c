/*
 * A245745  Primes p such that p - d is also prime, where d is the smallest
 *          nonzero digit of p.
 * https://oeis.org/A245745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 199, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9509 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th term of OEIS A245745:  
   primes p such that the smallest non‑zero digit d of p
   satisfies p - d is also prime.  The function caches the
   sequence terms and is agnostic to the order of calls.
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Utilities from the shared library */
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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

/* Static array caching the sequence terms.
   Terms are stored in 0‑based order: cache[0] = a(1).   */
static seq_t *cache = NULL;      /* array of found terms */
static size_t cache_size = 0;    /* number of terms stored */
static size_t cache_cap  = 0;    /* allocated capacity of cache array */
static idx_t last_checked = 1;   /* largest integer that was tested already */

/* Extend the cache until it holds at least n terms. */
static void generate_until(idx_t n)
{
    /* cache already large enough? */
    if (cache_size >= n)
        return;

    /* start checking from the first integer that has not yet been tested */
    idx_t cand = last_checked + 1;

    while (1) {
        /* skip non‑primes */
        if (!is_prime(cand)) {
            cand++;
            continue;
        }

        /* find the smallest non‑zero digit of candidate */
        seq_t d = 10;               /* more than any decimal digit */
        idx_t t = cand;
        while (t > 0) {
            seq_t dig = t % 10;
            if (dig != 0 && dig < d)
                d = dig;
            t /= 10;
        }

        /* p - d must also be prime */
        seq_t diff = (seq_t)cand - d;
        if (diff >= 2 && is_prime_big(diff)) {
            /* add to cache */
            if (cache_size == cache_cap)
                grow_seq(&cache, &cache_cap, cache_size + 1);
            cache[cache_size++] = (seq_t)cand;

            /* we now have enough terms? */
            if (cache_size >= n) {
                last_checked = cand;
                break;
            }
        }
        cand++;
    }
}

/* Public function expected by the harness. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset 1: a(1) is the first item */
        return 0;

    generate_until(n);
    return cache[n - 1];
}
