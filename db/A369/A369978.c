/*
 * A369978  Dirichlet inverse of sequence b(n) = 1+A083345(n), where
 *          A083345(n) = n' / gcd(n,n'), and n' stands for the arithmetic
 *          derivative of n, A003415.
 * https://oeis.org/A369978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the Dirichlet inverse of b(n)=1+A083345(n), where
 *            A083345(n) = n' / gcd(n,n') (arithmetic derivative). a(1)=1; for
 *            n>1, a(n) = -∑_{d|n, d<n} (1+A083345(n/d)) * a(d)
 * Range:     i8 n <= 63, i16 n <= 8959, i32 n <= 9845, i64 n <= 9845,
 *            i128 n <= 9845, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---- required includes for utilities ---- */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* ---- static state for caching ---- */
static seq_t   *a_cache = NULL;     /* Dirichlet inverse values */
static seq_t   *b_cache = NULL;     /* b(n) values */
static size_t   cache_cap = 0;      /* current allocated upper index (inclusive) */
static idx_t    computed  = 0;      /* maximum n already computed */

/* ---- smallest prime factor sieve ---- */
static uint32_t *spf = NULL;        /* spf[0..cap] */
static size_t   spf_cap = 0;        /* current capacity of spf */

/* ensure spf array is allocated up to at least n */
static void
ensure_spf(idx_t n)
{
    if (n <= spf_cap) return;
    size_t new_cap = (spf_cap ? spf_cap * 2 : 16);
    while (new_cap < (size_t)n) new_cap *= 2;
    uint32_t *new_spf = (uint32_t*)realloc(spf, (new_cap + 1) * sizeof(uint32_t));
    if (!new_spf) abort();
    spf = new_spf;
    for (size_t i = 0; i <= new_cap; ++i) spf[i] = 0;
    if (new_cap < 2) { spf_cap = new_cap; return; }
    spf[0] = spf[1] = 0;
    for (size_t i = 2; i <= new_cap; ++i) {
        if (!spf[i]) {
            spf[i] = (uint32_t)i;
            if ((uint64_t)i * i <= new_cap) {
                for (size_t j = i * i; j <= new_cap; j += i)
                    if (!spf[j]) spf[j] = (uint32_t)i;
            }
        }
    }
    spf_cap = new_cap;
}

/* ensure a_cache and b_cache are allocated up to at least n */
static void
ensure_cache_capacity(idx_t n)
{
    if (n <= cache_cap) return;
    size_t new_cap = (cache_cap ? cache_cap * 2 : 16);
    while (new_cap < (size_t)n) new_cap *= 2;
    seq_t *new_a = (seq_t*)realloc(a_cache, (new_cap + 1) * sizeof(seq_t));
    seq_t *new_b = (seq_t*)realloc(b_cache, (new_cap + 1) * sizeof(seq_t));
    if (!new_a || !new_b) abort();
    a_cache = new_a;
    b_cache = new_b;
    /* initialize newly allocated slots to zero */
    for (size_t i = cache_cap + 1; i <= new_cap; ++i)
        a_cache[i] = b_cache[i] = 0;
    cache_cap = new_cap;
}

/* compute all values from (computed+1) to N inclusive */
static void
compute_up_to(idx_t N)
{
    if (N <= computed) return;
    ensure_spf(N);
    ensure_cache_capacity(N);

    for (idx_t n = computed + 1; n <= N; ++n) {
        /* ---- compute b(n) = 1 + A083345(n) ---- */
        seq_t deriv = 0;
        if (n > 1) {
            idx_t orig = n;
            size_t temp = n;
            while (temp > 1) {
                uint32_t p = spf[temp];
                if (p == 0) p = (uint32_t)temp;   /* safety, should not happen */
                int exp = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++exp;
                }
                deriv += ((seq_t)exp) * ((seq_t)orig / (seq_t)p);
            }
        }
        seq_t g = gcd(deriv, (seq_t)n);
        /* gcd(0,1)=1 by definition, so deriv/g is integer */
        b_cache[n] = 1 + deriv / g;

        if (n == 1) {
            a_cache[1] = 1;
            continue;
        }

        /* ---- compute a(n) via recurrence ---- */
        seq_t sum = 0;
        for (idx_t d = 1; d * d <= n; ++d) {
            if (n % d == 0) {
                if (d < n) {
                    sum += b_cache[n / d] * a_cache[d];
                }
                idx_t q = n / d;
                if (q != d && q < n) {
                    sum += b_cache[n / q] * a_cache[q];
                }
            }
        }
        a_cache[n] = -sum;
    }
    computed = N;
}

/* external function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, index 0 is undefined */
    compute_up_to(n);
    return a_cache[n];
}
