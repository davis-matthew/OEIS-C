/*
 * A359664  Prime Maze Room 11, opposite parity of A059459 starting from prime
 *          room 11.
 * https://oeis.org/A359664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 9 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Prime Maze Room 11 sequence (OEIS A359664).
 * a(1) = 11.  For n>1, a(n) is the first prime obtained by
 * toggling a single bit of a(n-1), starting at the least
 * significant bit and moving upwards, skipping the numbers
 * 2, 3 and any prime that has already appeared.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* initialise the cache with the first term */
    if (cache_len == 0) {
        grow_seq(&cache, &cache_cap, (size_t)1);
        cache[0] = (seq_t)11;
        cache_len = 1;
    }

    /* Return early if the requested term is already cached */
    if ((idx_t)cache_len >= n) {
        return cache[n - 1];
    }

    /* Compute missing terms up to n */
    for (idx_t need = cache_len + 1; need <= n; ++need) {
        seq_t prev  = cache[cache_len - 1];
        seq_t mask  = (seq_t)1;
        seq_t next;

        /* search for the next term */
        while (true) {
            seq_t cand = prev ^ mask;

            if (cand != (seq_t)2 && cand != (seq_t)3 && is_prime_big(cand)) {
                /* ensure candidate has not appeared before */
                bool seen = false;
                for (size_t i = 0; i < cache_len; ++i) {
                    if (cache[i] == cand) { seen = true; break; }
                }
                if (!seen) {
                    next = cand;
                    break;
                }
            }

            /* advance to the next bit position */
            mask = mask * (seq_t)2;
        }

        /* extend cache if necessary and store the new term */
        if (cache_len == cache_cap) {
            grow_seq(&cache, &cache_cap, cache_len + 1);
        }
        cache[cache_len] = next;
        ++cache_len;
    }

    return cache[n - 1];
}
