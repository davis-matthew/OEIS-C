/*
 * A245970  Tower of 2's modulo n.
 * https://oeis.org/A245970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, look
 * Formula:   a(n) = 2^(φ(n)+a(φ(n))) mod n, a(n) = 0 if n is a power of 2.
 *            (OEIS A245970)
 * Range:     u8 n <= 267, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   totient64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/totient64.h"

/* Helper: test whether a seq_t value is a power of two. */
static bool is_power_of_two(seq_t x)
{
    return x != 0 && (x & (x - 1)) == 0;
}

/* Fast modular exponentiation for seq_t values. */
static seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod)
{
    seq_t result = 1;
    seq_t b = base % mod;
    while (exp)
    {
        if (exp & 1ULL)
            result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1ULL;
    }
    return result;
}

/* Simple cache structure.  */
typedef struct { idx_t n; seq_t v; } cache_entry;
static cache_entry *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;

static bool cache_get(idx_t n, seq_t *out)
{
    for (size_t i = 0; i < cache_len; ++i)
        if (cache[i].n == n)
        {
            *out = cache[i].v;
            return true;
        }
    return false;
}

static void cache_put(idx_t n, seq_t v)
{
    /* linear search for existing entry */
    for (size_t i = 0; i < cache_len; ++i)
        if (cache[i].n == n)
        {
            cache[i].v = v;
            return;
        }
    /* grow buffer if needed */
    if (cache_len == cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        cache = (cache_entry *)realloc(cache, new_cap * sizeof(cache_entry));
        cache_cap = new_cap;
    }
    cache[cache_len++] = (cache_entry){n, v};
}

/* Sequence function.  */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;            /* a(1) = 0 */

    if (is_power_of_two((seq_t)n))          /* power of 2 → 0 */
        return (seq_t)0;

    seq_t cached;
    if (cache_get(n, &cached)) return cached;

    uint64_t phi_n = totient64((uint64_t)n);
    seq_t a_phi = a((idx_t)phi_n);

    uint64_t exp = phi_n + (uint64_t)a_phi;
    seq_t val = mod_pow((seq_t)2, exp, (seq_t)n);

    cache_put(n, val);
    return val;
}
