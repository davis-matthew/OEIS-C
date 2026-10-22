/*
 * A345343  Numbers that yield a prime when any single digit is replaced by
 *          its digital complement.
 * https://oeis.org/A345343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   a(n) = the nth positive integer that, when any single digit is *
 *            replaced by its digital complement (10‑d if d>0, 0 otherwise),
 *            yields a * prime
 * Range:     u8 n <= 13, u16 n <= 51, u32 n <= 51, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

typedef uint64_t idx_t;   /* These types are supplied by the build system. */

/* Pre‑compute powers of 10 for digit replacement. */
static seq_t pow10[32];
static bool pow10_inited = false;
static void init_pow10(void)
{
    if (pow10_inited) return;
    pow10[0] = (seq_t)1;
    for (size_t i = 1; i < 32; ++i) pow10[i] = pow10[i - 1] * (seq_t)10;
    pow10_inited = true;
}

/* Return true if num satisfies the primeness property. */
static bool valid_candidate(seq_t num)
{
    uint8_t digits[32];
    size_t len = 0;
    seq_t t = num;
    if (t == (seq_t)0) { digits[0] = 0; len = 1; }
    while (t != (seq_t)0) {
        digits[len++] = (uint8_t)(t % (seq_t)10);
        t /= (seq_t)10;
    }
    for (size_t pos = 0; pos < len; ++pos) {
        uint8_t d  = digits[pos];
        uint8_t cop = (d == 0) ? 0 : 10 - d;
        seq_t mul = pow10[pos];
        seq_t rep = num - (seq_t)d * mul + (seq_t)cop * mul;
        if (!is_prime_big(rep)) return false;
    }
    return true;
}

/* Memoised sequence. */
static seq_t *cache = NULL;     /* storing terms, 0‑based index */
static size_t cache_len = 0;    /* how many terms currently cached */
static size_t cache_cap = 0;    /* allocated capacity */

/* Ensure the cache can hold at least 'need' items. */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) { /* allocation error – abort gracefully */
        abort();
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Find the nth term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined: offset 1 */
    if (!pow10_inited) init_pow10();

    size_t target = (size_t)n;   /* zero‑based count desired */
    if (cache_len < target) {
        ensure_cache_capacity(target);
        seq_t candidate = (cache_len == 0) ? (seq_t)2 : cache[cache_len - 1] + (seq_t)1;
        while (cache_len < target) {
            if (valid_candidate(candidate)) {
                cache[cache_len++] = candidate;
                candidate++;
                continue;
            }
            candidate++;
        }
    }
    return cache[n - 1];
}
