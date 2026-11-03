/*
 * A386155  Primes having only {2, 4, 6, 7} as digits.
 * https://oeis.org/A386155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 62, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 59 terms through n = 9323 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A386155:
 * primes whose decimal representation uses only the digits {2,4,6,7}. */

#include <stdbool.h>
#include <stdlib.h>
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
    /* static state for memoisation */
    static seq_t *cache = NULL;          /* array of found primes */
    static size_t cache_cap = 0;          /* allocated capacity of cache   */
    static idx_t found = 0;              /* number of primes stored      */
    static int cur_len = 0;              /* current number of digits     */
    static int digits[128];              /* indices in [0..3] for digits */

    /* mapping from index to actual decimal digit */
    static const int digit_map[4] = { 2, 4, 6, 7 };

    /* initialise on first call */
    if (cache == NULL) {
        cache_cap = 64;
        cache = mem_new(seq_t, cache_cap);
        cur_len = 1;
        for (int i = 0; i < cur_len; ++i) digits[i] = 0;
    }

    /* produce primes until we have at least n of them */
    while (found < n) {
        /* build the current candidate number */
        seq_t val = 0;
        for (int i = 0; i < cur_len; ++i) {
            val = val * 10 + (seq_t)digit_map[digits[i]];
        }

        /* test primality */
        if (is_prime_big(val)) {
            /* store into cache, growing if necessary */
            if ((size_t)found >= cache_cap) {
                cache_cap <<= 1;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[found++] = val;
        }

        /* advance to the next candidate in base‑4 order */
        int pos = cur_len - 1;
        while (pos >= 0) {
            if (digits[pos] < 3) {
                ++digits[pos];
                break;
            } else {
                digits[pos] = 0;
                --pos;
            }
        }
        if (pos < 0) {              /* exhausted all numbers of this length */
            ++cur_len;
            for (int i = 0; i < cur_len; ++i) digits[i] = 0;
        }
    }

    /* return the requested term */
    return cache[n - 1];
}
