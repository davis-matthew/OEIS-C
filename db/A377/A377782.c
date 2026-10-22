/*
 * A377782  First-differences of A031218(n) = greatest number <= n that is 1
 *          or a prime-power.
 * https://oeis.org/A377782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A377782(n) = g(n+1) – g(n), where g(m) = max{k ≤ m : k = 1 or (k
 *            is a prime power)}
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Cache for g(m) values */
static seq_t *g_cache = NULL;      /* g_cache[i] holds g(i) */
static size_t  g_cap   = 0;        /* current allocated size (≥0) */

/* Test whether a 64‑bit integer n is 1 or a prime power (exponent ≥1). */
static bool is_prime_power_uint64(uint64_t n)
{
    if (n == 1) return true;          /* 1 is explicitly allowed */
    uint64_t prime = 0;
    for (uint64_t d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            if (prime == 0) prime = d;
            else if (d != prime) return false;
            do { n /= d; } while (n % d == 0);
            if (n == 1) break;
        }
    }
    if (n > 1) {
        if (prime == 0) return true;  /* n itself is prime */
        if (prime != n) return false;
    }
    return true;
}

/* Ensure g_cache is allocated up to index need (inclusive). */
static void ensure_g(idx_t need)
{
    if (need + 1 <= (idx_t)g_cap) return;   /* already enough */
    size_t new_cap = (size_t)need + 2;      /* indexes 0 … need */
    seq_t *tmp = (seq_t *)realloc(g_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);          /* allocation failure */
    g_cache = tmp;

    /* Initialize first value if cache was empty. */
    if (g_cap == 0) g_cache[0] = (seq_t)0;

    seq_t last = (seq_t)(g_cap - 1 < 0 ? 0 : g_cache[g_cap - 1]);

    for (size_t i = g_cap; i < new_cap; ++i) {
        seq_t val = (seq_t)i;
        if (is_prime_power_uint64((uint64_t)i)) last = val;
        g_cache[i] = last;
    }
    g_cap = new_cap;
}

/* External function implementing the sequence. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */
    ensure_g(n + 1);                     /* need g[n] and g[n+1] */
    seq_t gn   = g_cache[(size_t)n];
    seq_t gnp1 = g_cache[(size_t)(n + 1)];
    return gnp1 - gn;
}
