/*
 * A386099  Primes having only {1, 2, 6, 7} as digits.
 * https://oeis.org/A386099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 126, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 9601 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = n-th prime whose decimal digits are only {1,2,6,7}
   by enumerating numbers with those digits and testing primality. */
#include <stdbool.h>          /* bool */
#include <stddef.h>           /* size_t, NULL */
#include <stdlib.h>           /* malloc, realloc, abort */
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

static seq_t *prime_cache = NULL;     /* cached primes */
static size_t  cache_cap   = 0;        /* capacity of cache array    */
static size_t  cache_size  = 0;        /* number of primes stored    */

/* ensure cache has capacity for at least `needed` elements */
static void ensure_cache(size_t needed) {
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, prime_cache, new_cap);
    if (!tmp) abort();                     /* out of memory */
    prime_cache = tmp;
    cache_cap = new_cap;
}

/* generator state for numbers consisting only of digits 1,2,6,7 */
static idx_t cur_len = 1;                /* current digit length          */
static idx_t cur_idx = 0;                /* index within current length   */
static idx_t cur_pow4 = 4;               /* 4^cur_len (number of candidates of current length) */

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* undefined for index 0          */

    const int digit_map[4] = {1, 2, 6, 7};

    while (cache_size < (size_t)n) {
        /* build the next candidate number consisting only of allowed digits */
        seq_t num = 0;
        for (int pos = 0; pos < (int)cur_len; ++pos) {
            idx_t shift = ((idx_t)cur_len - 1 - (idx_t)pos) << 1; /* 2* (cur_len-1-pos)  */
            int di = (int)((cur_idx >> shift) & 3);
            num = num * 10 + digit_map[di];
        }

        if (is_prime_big(num)) {
            ensure_cache(cache_size + 1);
            prime_cache[cache_size++] = num;
        }

        /* advance to next candidate */
        cur_idx++;
        if (cur_idx == cur_pow4) {       /* finished current length      */
            cur_len++;
            cur_idx = 0;
            cur_pow4 <<= 2;               /* multiply by 4                */
        }
    }
    return prime_cache[n - 1];
}
