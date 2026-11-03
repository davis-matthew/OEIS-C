/*
 * A386172  Primes having only {3, 4, 5, 9} as digits.
 * https://oeis.org/A386172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 143, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 9754 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes whose decimal representation uses only digits 3,4,5,9 */

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

static const uint8_t allowed_digits[4] = {3, 4, 5, 9};

/* powers of four, up to the first overflow (4^19 < 2^63 < 4^20) */
static size_t pow4[32];
static bool pow4_inited = false;

static void init_pow4(void)
{
    if (pow4_inited) return;
    pow4[0] = 1;
    for (size_t i = 1; i < 32; ++i) {
        if (pow4[i - 1] > (SIZE_MAX / 4)) {
            pow4[i] = 0;          /* overflow marker (never used) */
            break;
        }
        pow4[i] = pow4[i - 1] * 4;
    }
    pow4_inited = true;
}

/* dynamic array for cached primes */
static seq_t *prime_cache = NULL;
static size_t prime_cache_len = 0;
static size_t prime_cache_cap = 0;

static void ensure_prime_cache(size_t need)
{
    if (need <= prime_cache_cap) return;
    size_t new_cap = prime_cache_cap ? prime_cache_cap * 2 : 64;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, prime_cache, new_cap);
    prime_cache = tmp;
    prime_cache_cap = new_cap;
}

/* state of incremental generation */
static size_t current_length = 1; /* number of digits currently being processed */
static size_t idx_in_length = 0;  /* next candidate index within current_length */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, guard */

    size_t target = (size_t)n;
    if ((size_t)n <= prime_cache_len) {
        return prime_cache[(size_t)n - 1];
    }

    init_pow4();
    ensure_prime_cache(target);

    while (prime_cache_len < target) {
        size_t total = pow4[current_length];
        if (total == 0) {          /* overflow – never should happen */
            break;
        }

        if (idx_in_length >= total) { /* finished this length, move on */
            current_length++;
            idx_in_length = 0;
            continue;
        }

        /* Build the candidate number of current_length digits */
        seq_t val = 0;
        size_t idx = idx_in_length;
        for (size_t pos = 0; pos < current_length; ++pos) {
            size_t digit_index = (idx / pow4[current_length - 1 - pos]) % 4;
            val = val * (seq_t)10 + (seq_t)allowed_digits[digit_index];
        }

        if (is_prime_big(val)) {
            prime_cache[prime_cache_len++] = val;
            ++idx_in_length;           /* advance past this candidate */
            if (prime_cache_len == target) {
                break;
            }
            continue;
        }

        ++idx_in_length;               /* move to next candidate */
    }

    return prime_cache[(size_t)n - 1];
}
