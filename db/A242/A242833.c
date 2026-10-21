/*
 * A242833  Primes formed by the initial digits of the decimal expansion of
 *          1/19, starting at the first nonzero digit in the expansion.
 * https://oeis.org/A242833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: primes formed by prefixes of the decimal expansion
   of 1/19, starting after the leading zero. Each term is the smallest
   prefix (in increasing length) that is prime. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

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
        seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
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
