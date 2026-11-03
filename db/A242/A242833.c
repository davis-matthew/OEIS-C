/*
 * A242833  Primes formed by the initial digits of the decimal expansion of
 *          1/19, starting at the first nonzero digit in the expansion.
 * https://oeis.org/A242833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Validated: standard, pass, 6 terms through n = 6 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: primes formed by prefixes of the decimal expansion
   of 1/19, starting after the leading zero. Each term is the smallest
   prefix (in increasing length) that is prime. */
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

static const char cycle_str[] = "526315789473684210";
static const size_t CYCLE_LEN = sizeof(cycle_str) - 1;

static seq_t *prime_cache = NULL;   /* primes stored 1‑based */
static size_t cache_len = 0;
static size_t cache_cap = 0;

static seq_t current_prefix = (seq_t)0;
static idx_t digits_appended = 0;
static bool initialized = false;

static void ensure_cache_capacity(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = mem_resize(seq_t, prime_cache, new_cap);
        if (!tmp) exit(1);
        prime_cache = tmp;
        cache_cap = new_cap;
    }
}

static void generate_until(idx_t target)
{
    while (cache_len < target) {
        size_t pos = digits_appended % CYCLE_LEN;
        int digit = cycle_str[pos] - '0';
        current_prefix = current_prefix * 10 + (seq_t)digit;
        ++digits_appended;

        if (is_prime_big(current_prefix)) {
            ensure_cache_capacity();
            prime_cache[cache_len++] = current_prefix;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1 */

    if (!initialized) {
        initialized = true;
        generate_until(1);          /* prime 5 */
    }

    if (n > cache_len) {
        generate_until(n);
    }
    return prime_cache[n - 1];
}
