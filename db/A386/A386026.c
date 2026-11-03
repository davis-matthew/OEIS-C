/*
 * A386026  Primes having only {0, 1, 3, 9} as digits.
 * https://oeis.org/A386026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 183, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 8933 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute: a(n) = the nth prime whose decimal representation
   contains only the digits 0, 1, 3, or 9.  The function
   generates all admissible numbers by length, tests each one
   for primality (via is_prime_big), and caches the sequence.
   It returns cached results for any order of calls. */

#include <stdlib.h>
#include <stdbool.h>
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

/* Cache of already found primes */
static seq_t *cache   = NULL;   /* array of primes */
static size_t cache_cap = 0;    /* allocated capacity */
static idx_t  cache_cnt  = 0;   /* number stored */
static int    max_len_done = 0; /* longest length fully processed */

/* Ensure cache has space for at least 'needed' elements. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed) new_cap <<= 1;
    cache = mem_resize(seq_t, cache, new_cap);
    cache_cap = new_cap;
}

/* Generate all numbers of a given length made of digits {0,1,3,9}
   (first digit not 0, last digit not even).  On a completed number
   that is prime and not divisible by 3 (except for the number 3),
   it is appended to the cache. */
static void generate_len(int len, int pos, seq_t val, int sum_mod3)
{
    if (pos == len) {
        if (val == 3 || sum_mod3 != 0) {
            if (is_prime_big(val)) {
                grow_cache(cache_cnt + 1);
                cache[cache_cnt++] = val;
            }
        }
        return;
    }

    const seq_t *digits;
    size_t nd;

    if (pos == 0) {                     /* first digit */
        static const seq_t d0[3] = {1, 3, 9};
        digits = d0; nd = 3;
    } else if (pos == len - 1) {        /* last digit (must be odd) */
        static const seq_t d1[3] = {1, 3, 9};
        digits = d1; nd = 3;
    } else {                            /* middle digits */
        static const seq_t d2[4] = {0, 1, 3, 9};
        digits = d2; nd = 4;
    }

    for (size_t i = 0; i < nd; ++i) {
        generate_len(len, pos + 1,
                     val * 10 + digits[i],
                     (sum_mod3 + (int)digits[i]) % 3);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* undefined for n=0 */

    if (n <= cache_cnt) return cache[n - 1];

    idx_t target = n;

    for (int len = max_len_done + 1; ; ++len) {
        generate_len(len, 0, 0, 0);         /* generate all of this length */
        max_len_done = len;                /* we have finished this length */
        if (cache_cnt >= target) break;
    }
    return cache[target - 1];
}
