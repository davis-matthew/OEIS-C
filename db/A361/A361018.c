/*
 * A361018  Parity of A361017, where A361017 is the Dirichlet inverse of
 *          Thue-Morse sequence, A010060.
 * https://oeis.org/A361018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8305, u16 n <= 8305, u32 n <= 8305, u64 n <= 8305,
 *            u128 n <= 8305, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Parity of A361017:  a(n) = parity of the Dirichlet inverse of the Thue‑Morse sequence (A010060) */
/* This implements the recurrence:  g(1)=1; for n>1, g(n) ≡ Σ_{d|n, d≥2} f(d)·g(n/d)  (mod 2)   */
/* where f(d) is the parity of the binary digit sum of d (i.e. Thue‑Morse value of d).  The function returns a(n)=g(n) mod 2. */

#include <stdlib.h>
#include <stdint.h>

/* cache for g(n) modulo 2 */
static uint8_t *g_cache = NULL;
static size_t   cache_cap = 0;
static idx_t   last_filled = 0;   /* highest n for which g_cache[n] is defined */

/* ensure that g_cache[1..n] is computed */
static void ensure_cache(idx_t n)
{
    if (n < 1) return;

    if (g_cache == NULL) {
        cache_cap = (size_t)n + 1;
        g_cache = (uint8_t*)malloc(cache_cap);
        if (g_cache == NULL) return;            /* allocation failure – caller will get 0 */
        g_cache[0] = 0;                          /* unused */
        g_cache[1] = 1;                          /* g(1)=1 */
        last_filled = 1;
    } else if ((size_t)n + 1 > cache_cap) {
        uint8_t *tmp = (uint8_t*)realloc(g_cache, (size_t)n + 1);
        if (tmp == NULL) return;
        g_cache = tmp;
        cache_cap = (size_t)n + 1;
    }

    /* compute missing values */
    for (idx_t k = last_filled + 1; k <= n; ++k) {
        uint8_t val = 0;                         /* accumulator for parity */
        for (idx_t d = 2; d * d <= k; ++d) {
            if (k % d == 0) {
                if ((__builtin_popcountll(d) & 1)) val ^= g_cache[(size_t)(k / d)];
                idx_t e = k / d;
                if (e != d && e > 1 && (__builtin_popcountll(e) & 1))
                    val ^= g_cache[(size_t)(k / e)];
            }
        }
        /* include divisor d = k itself */
        if (__builtin_popcountll(k) & 1) val ^= g_cache[1];
        g_cache[k] = val & 1u;
    }
    last_filled = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_cache(n);
    return (seq_t)(g_cache[(size_t)n] & 1u);
}
