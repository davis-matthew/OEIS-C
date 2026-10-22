/*
 * A344621  Primes p such that there is no positive integer a with 2*a, a^2-1
 *          and a^2+1 not only smaller than p but also quadratic nonresidues
 *          modulo p.
 * https://oeis.org/A344621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A344621: primes p such that there is no positive integer a with
   2*a, a^2‑1 and a^2+1 all smaller than p and simultaneously quadratic
   nonresidues modulo p. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mod_pow.h"

static bool is_prime(uint64_t n)
{
    if (n < 2)           return false;
    if (n % 2 == 0)      return n == 2;
    uint64_t d = 3;
    while (d * d <= n) {
        if (n % d == 0)  return false;
        d += 2;
    }
    return true;
}

static bool satisfies_property(uint64_t p)
{
    if (p == 2) return true;     /* no admissible a for p = 2 */

    seq_t modp = (seq_t)p;
    seq_t nonres = modp - 1;     /* -1 mod p */
    uint64_t exp = (p - 1) / 2;  /* exponent for Legendre symbol */

    for (uint64_t a = 1; ; ++a) {
        uint64_t two_a = 2 * a;
        if (two_a >= p) break;

        uint64_t a2 = a * a;
        uint64_t r2 = a2 - 1;
        if (r2 >= p) break;
        uint64_t r3 = a2 + 1;
        if (r3 >= p) break;

        seq_t l1 = mod_pow((seq_t)two_a, exp, modp);
        if (l1 != nonres)         continue;
        seq_t l2 = mod_pow((seq_t)r2, exp, modp);
        if (l2 != nonres)         continue;
        seq_t l3 = mod_pow((seq_t)r3, exp, modp);
        if (l3 == nonres)         return false; /* counter‑example found */
    }
    return true;   /* no counter‑example found */
}

static seq_t *cache = NULL;        /* cached primes satisfying property */
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t last_checked = 0;  /* last integer examined for primality */

static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    if (cache) {
        memcpy(new_arr, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
    cache = new_arr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1 */

    while (cache_len < (size_t)n) {
        ++last_checked;
        if (!is_prime(last_checked)) continue;

        if (!satisfies_property(last_checked)) continue;

        if (cache_len == cache_cap) grow_cache();
        cache[cache_len++] = (seq_t)last_checked;
    }
    return cache[n - 1];
}
