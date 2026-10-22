/*
 * A378222  Number of ordered factorizations of the odd part of n into factors
 *          > 1.
 * https://oeis.org/A378222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A074206(A000265(n)) – number of ordered factorizations of
 *            the odd part of n into factors > 1, with a(1) = 1
 * Range:     u8 n <= 2834, u16 n <= 9873, u32 n <= 9873, u64 n <= 9873,
 *            u128 n <= 9873, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *g_cache = NULL;
static size_t g_cap    = 0;

/* Ensure g_cache has space for indices up to n (inclusive). */
static void ensure_g_cache(idx_t n)
{
    if (g_cap <= n) {
        size_t new_cap = g_cap ? g_cap << 1 : 256;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(g_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        for (size_t i = g_cap; i < new_cap; ++i) tmp[i] = 0;
        g_cache = tmp;
        g_cap   = new_cap;
    }
}

/* Recursively compute the number of ordered factorizations of m (m >= 1). */
static seq_t g_compute(idx_t m)
{
    if (m == 1) return 1;
    ensure_g_cache(m);
    if (g_cache[m] != 0) return g_cache[m];

    seq_t total = 0;
    for (idx_t d = 2; d <= m / d; ++d) {
        if (m % d == 0) {
            total += g_compute(m / d);   /* using divisor d          */
            if (d != m / d)              /* using paired divisor m/d */
                total += g_compute(d);
        }
    }
    total += 1;  /* divisor m itself contributes g(1) = 1 */
    g_cache[m] = total;
    return total;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined but safe */
    /* Extract the odd part of n. */
    idx_t odd = n;
    while ((odd & 1) == 0) odd >>= 1;

    return g_compute(odd);
}
