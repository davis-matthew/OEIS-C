/*
 * A327799  Expansion of 1 / (1 + Sum_{i>=1} Sum_{j=1..i} x^(i*j)).
 * https://oeis.org/A327799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 29, i16 n <= 53, i32 n <= 53, i64 n <= 53, i128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A327799
   a(0) = 1;
   a(n) = - Σ_{k=1..n} A038548(k) * a(n-k)
   where A038548(k) = (d(k)+1)/2 and d(k) is the divisor‑count function. */

#include <stdlib.h>

static idx_t *spf    = NULL; /* smallest prime factor table */
static size_t spf_cap = 0;    /* current allocated size */

static void ensure_spf(idx_t n)
{
    if (spf_cap > n) return;

    idx_t *new_spf = (idx_t *)realloc(spf, (n + 1) * sizeof(idx_t));
    if (!new_spf) exit(EXIT_FAILURE);
    spf   = new_spf;
    spf_cap = n + 1;

    for (idx_t i = 0; i <= n; ++i) spf[i] = i;
    if (n < 2) return;

    for (idx_t i = 2; i * i <= n; ++i) {
        if (spf[i] != i) continue;
        for (idx_t j = i * i; j <= n; j += i)
            if (spf[j] == j) spf[j] = i;
    }
}

static idx_t *u_cache   = NULL; /* (d(k)+1)/2 values */
static size_t u_cap     = 0;     /* current allocated size */

static void ensure_u(idx_t n)
{
    if (u_cap > n) return;

    idx_t *new_u = (idx_t *)realloc(u_cache, (n + 1) * sizeof(idx_t));
    if (!new_u) exit(EXIT_FAILURE);
    u_cache = new_u;

    ensure_spf(n);

    /* divisor counts for indices we have not yet computed */
    static idx_t *d      = NULL;
    static size_t d_cap  = 0;
    if (d_cap < (size_t)(n + 1)) {
        idx_t *new_d = (idx_t *)realloc(d, (n + 1) * sizeof(idx_t));
        if (!new_d) exit(EXIT_FAILURE);
        d = new_d;
        d_cap = n + 1;
    }

    u_cache[0] = 0; /* not used */

    size_t start = (u_cap > 1) ? u_cap : 1;
    for (size_t ii = start; ii <= (size_t)n; ++ii) {
        idx_t i = (idx_t)ii;
        if (i == 1) {
            d[1]        = 1;
            u_cache[1]  = 1;
            continue;
        }
        idx_t p   = spf[i];
        idx_t m   = i / p;
        idx_t exp = 1;
        while (m % p == 0) {
            m /= p;
            exp++;
        }
        d[i] = d[m] * (exp + 1);
        u_cache[i] = (d[i] + 1) >> 1;
    }
    u_cap = (size_t)n + 1;
}

static seq_t *a_cache = NULL; /* cached a(n) values */
static size_t a_cap   = 0;     /* current allocated size */

seq_t a(idx_t n)
{
    if (a_cap <= (size_t)n) {
        ensure_u(n);

        seq_t *new_a = (seq_t *)realloc(a_cache, (n + 1) * sizeof(seq_t));
        if (!new_a) exit(EXIT_FAILURE);
        a_cache = new_a;

        size_t old_cap = a_cap;
        if (old_cap == 0) {          /* first call */
            a_cache[0] = 1;
            old_cap = 1;
        }

        for (size_t i = old_cap; i <= (size_t)n; ++i) {
            seq_t sum = 0;
            for (size_t k = 1; k <= i; ++k) {
                sum += (seq_t)u_cache[k] * a_cache[i - k];
            }
            a_cache[i] = -sum;
        }
        a_cap = n + 1;
    }
    return a_cache[n];
}
