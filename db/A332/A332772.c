/*
 * A332772  Numbers k > 0 such that 30k +- 7 is prime.
 * https://oeis.org/A332772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A332772: Numbers k > 0 such that 30k ± 7 is prime.
   The function returns the n-th such k (1‑based).
   It builds the sequence lazily and memoises results, so calls may come
   in any order. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include <stdint.h>
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

seq_t a(idx_t n)
{
    /* static cache of found k values */
    static seq_t *cache = NULL;      /* holds the k's */
    static size_t cache_cnt = 0;     /* number of elements stored */
    static size_t cache_cap = 0;     /* allocated capacity */
    static idx_t next_k = 1;         /* next k to test */

    if (n == 0) /* the off‑by‑one offset, not used in OEIS */
        return 0;

    /* grow the cache until we have at least n elements */
    while (cache_cnt < (size_t)n) {
        /* find the next k with both 30k-7 and 30k+7 prime */
        for (;;) {
            uint64_t v1 = 30u * (uint64_t)next_k - 7u;
            uint64_t v2 = 30u * (uint64_t)next_k + 7u;

            if (is_prime_big((seq_t)v1) && is_prime_big((seq_t)v2)) {
                /* store the found k */
                if (cache_cnt == cache_cap) {
                    cache_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
                    cache = mem_resize(seq_t, cache, cache_cap);
                }
                cache[cache_cnt++] = (seq_t)next_k;
                ++next_k;
                break;          /* move on to the next needed term */
            }
            ++next_k;
        }
    }

    return cache[(size_t)n - 1];
}
