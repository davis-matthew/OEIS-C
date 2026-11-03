/*
 * A386125  Primes having only {1, 4, 6, 9} as digits.
 * https://oeis.org/A386125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth prime whose decimal digits are from {1,4,6,9}. The
 *            function generates numbers using only these digits in increasing
 *            order and tests each for primality until it accumulates the
 *            requested number of primes
 * Range:     u8 n <= 7, u16 n <= 133, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 9968 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

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

static const int DIGITS[4] = {1, 4, 6, 9};

/* Generator state that yields all numbers consisting only of DIGITS in
   ascending numeric order. */
typedef struct {
    int len;               /* current number of digits */
    uint8_t idx[20];       /* indices into DIGITS for each position */
} gen_t;

static gen_t generator = { .len = 1,
                           .idx = {0} };

static uint64_t next_allowed_number(void) {
    /* Build the current number from the digit indices. */
    uint64_t val = 0;
    for (int i = 0; i < generator.len; ++i) {
        val = val * 10 + DIGITS[generator.idx[i]];
    }

    /* Increment the mixed‑radix counter for the next call. */
    int pos = generator.len - 1;
    while (pos >= 0) {
        if (++generator.idx[pos] < 4) {
            break;
        }
        generator.idx[pos] = 0;
        pos--;
    }
    if (pos < 0) {                 /* overflow: increase the number of digits */
        generator.len++;
        for (int i = 0; i < generator.len; ++i) {
            generator.idx[i] = 0;
        }
    }

    return val;
}

/* Dynamic array of primes found so far. */
static seq_t *prime_cache = NULL;
static size_t prime_cache_len = 0;
static size_t prime_cache_cap = 0;

/* Return the nth prime with digits in {1,4,6,9}. */
seq_t a(idx_t n) {
    if (n == 0) {
        return 0;   /* undefined for n < 1; harmless */
    }

    /* Grow the cache until it contains at least n primes. */
    while (prime_cache_len < (size_t)n) {
        uint64_t val = next_allowed_number();
        /* Only numbers ending in 1 or 9 can be prime (except 2) – filter out
           early to avoid unnecessary primality tests. */
        if ((val % 10 == 1) || (val % 10 == 9)) {
            if (is_prime_big((seq_t)val)) {
                grow_seq(&prime_cache, &prime_cache_cap, prime_cache_len + 1);
                prime_cache[prime_cache_len++] = (seq_t)val;
            }
        }
    }

    return prime_cache[n - 1];
}
