/*
 * A333995  a(n) = number of distinct composite numbers in the n X n
 *          multiplication table that are not in the n-1 X n-1 multiplication
 *          table.
 * https://oeis.org/A333995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed incrementally by marking all distinct products up
 *            to n² and counting the new composite numbers introduced by the
 *            nth row and column of the multiplication table
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/is_prime.h"   /* provides: bool is_prime(idx_t x); */

static inline bool bit_get(const uint64_t *bits, idx_t v)
{
    return (bits[v >> 6] >> (v & 63)) & 1ull;
}

static inline void bit_set(uint64_t *bits, idx_t v)
{
    bits[v >> 6] |= 1ull << (v & 63);
}

/* Static state maintained between calls. */
static uint64_t            g_max_n      = 0;      /* largest n processed so far */
static seq_t              *g_cache      = NULL;   /* a(0)…a(g_max_n) */
static uint64_t           *g_seen_bits  = NULL;   /* bitset of seen products   */
static size_t             g_seen_words = 0;       /* number of 64‑bit words    */

/* Ensure g_seen_bits can address all products up to max_n². */
static void ensure_seen_bits(idx_t max_n)
{
    idx_t max_val = max_n * max_n;
    size_t need = (size_t)(max_val >> 6) + 1;   /* words of 64‑bit */

    if (g_seen_bits == NULL) {
        g_seen_bits = (uint64_t *)calloc(need, sizeof(uint64_t));
        g_seen_words = need;
    } else if (need > g_seen_words) {
        uint64_t *tmp = (uint64_t *)realloc(g_seen_bits, need * sizeof(uint64_t));
        if (tmp == NULL) exit(1);                 /* allocation failure */
        memset(tmp + g_seen_words, 0, (need - g_seen_words) * sizeof(uint64_t));
        g_seen_bits = tmp;
        g_seen_words = need;
    }
}

/* Ensure g_cache can hold values up to max_n. */
static void ensure_cache(idx_t max_n)
{
    size_t new_cap = (size_t)(max_n + 1);
    if (g_cache == NULL) {
        g_cache = (seq_t *)calloc(new_cap, sizeof(seq_t));
    } else {
        seq_t *tmp = (seq_t *)realloc(g_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(1);
        g_cache = tmp;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* sequence defined for n ≥ 1 */

    if (n > g_max_n) {
        ensure_seen_bits(n);
        ensure_cache(n);

        for (idx_t cur = g_max_n + 1; cur <= n; ++cur) {
            seq_t count = 0;
            for (idx_t k = 1; k <= cur; ++k) {
                idx_t prod = cur * k;                     /* ≤ cur² ≤ n² */
                if (!bit_get(g_seen_bits, prod)) {         /* first time */
                    bit_set(g_seen_bits, prod);
                    if (prod > 1 && !is_prime(prod)) {     /* composite */
                        ++count;
                    }
                }
            }
            g_cache[cur] = count;
        }
        g_max_n = n;
    }
    return g_cache[n];
}
