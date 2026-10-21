/*
 * A283334  G.f.: 1/(1 - x*d/dx log(eta(x))), where eta(x) is Dedekind's
 *          eta(q) function without the q^(1/24) factor.
 * https://oeis.org/A283334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -Sum_{k=0..n-1} sigma(n-k) * a(k), a(0) = 1
 * Range:     i8 n <= 20, i16 n <= 45, i32 n <= 90, i64 n <= 181,
 *            i128 n <= 363, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (sigma(m) = sum of divisors of m) */
#include <stdlib.h>

/* Cached sequence values a[0..] and divisor sums sigma[0..] */
static seq_t *s_a     = NULL; /* a[0]..a[last] */
static size_t s_asize = 0;    /* number of stored a values (last+1) */

static seq_t *s_sigma = NULL; /* sigma[0]..sigma[last]  (sigma[0] = 0) */
static size_t s_sigsize = 0;  /* number of stored sigma values */

/* Build sigma[1..N] (sigma[0] stays zero). */
static void build_sigma(idx_t N)
{
    size_t new_size = (size_t)N + 1;           /* includes index 0 */
    seq_t *new_sigma = (seq_t*)calloc(new_size, sizeof(seq_t));
    if (!new_sigma) return;                   /* allocation failed – ignore */

    for (idx_t i = 1; i <= N; ++i) {
        seq_t add = (seq_t)i;
        for (idx_t j = i; j <= N; j += i)
            new_sigma[(size_t)j] += add;
    }

    free(s_sigma);
    s_sigma   = new_sigma;
    s_sigsize = new_size;
}

/* Extend the sequence array to cover indices 0..N (inclusive). */
static void extend_a(idx_t N)
{
    if ((size_t)N >= s_asize) {
        size_t oldsize = s_asize;
        size_t newsize = (size_t)N + 1;
        seq_t *new_a = (seq_t*)realloc(s_a, newsize * sizeof(seq_t));
        if (!new_a) return;                  /* allocation failed – ignore */

        if (oldsize == 0)
            new_a[0] = 1;                    /* a(0) = 1 */

        s_a = new_a;

        /* Compute new values if any. */
        for (size_t i = oldsize; i <= (size_t)N; ++i) {
            if (i == 0) continue;            /* already set */
            seq_t sum = 0;
            for (idx_t j = 1; j <= (idx_t)i; ++j)
                sum += s_sigma[(size_t)j] * s_a[(size_t)(i - j)];
            s_a[(size_t)i] = -sum;
        }

        s_asize = newsize;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n >= (idx_t)s_sigsize)
        build_sigma(n);
    if (n >= (idx_t)s_asize)
        extend_a(n);
    return s_a[(size_t)n];
}
