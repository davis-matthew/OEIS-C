/*
 * A350433  G.f. A(x) satisfies: R(x^2*A(x)) = x^3 - x^4, where R(A(x)) = x.
 * https://oeis.org/A350433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{m=1}^{⌊(n+2)/3⌋} a(m) * (−1)^{n+2−3m} * binom(m,
 *            n+2−3m), with a(1) = 1. The recurrence is derived from the
 *            function equation R(x²A(x)) = x³−x⁴, where R is the reversion of
 *            A
 * Range:     i8 n <= 30, i16 n <= 45, i32 n <= 97, i64 n <= 164,
 *            i128 n <= 335, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* --- small binomial coefficient ------------------------------------- */
static seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* --- dynamic cache for the sequence ------------------------------- */
static seq_t *cache   = NULL;
static size_t  cap    = 0;
static idx_t  hmax    = 0;

static void ensure_cache(idx_t n)
{
    if (n < cap) return;                /* already large enough */

    size_t new_cap = cap ? cap : 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    seq_t *tmp;
#if defined(__cplusplus)
    tmp = new seq_t[new_cap];
#else
    tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
    if (!tmp) return;                   /* allocation failed – abort */

    /* copy old values */
    if (cache) {
        for (size_t i = 0; i < cap; ++i) tmp[i] = cache[i];
#if defined(__cplusplus)
        delete [] cache;
#else
        free(cache);
#endif
    }

    /* initialise new slots to 0 */
    for (size_t i = cap; i <= (size_t)n; ++i) tmp[i] = 0;

    cache = tmp;
    cap   = new_cap;
}

/* --- sequence function --------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for OEIS offset 1 */

    ensure_cache(n);

    /* initialise the first two terms if not yet done */
    if (hmax < 1) {
        cache[0] = 0;
        cache[1] = 1;
        hmax    = 1;
    }

    for (idx_t j = hmax + 1; j <= n; ++j) {
        seq_t sum = 0;
        idx_t upper = (j + 2) / 3;
        for (idx_t m = 1; m <= upper; ++m) {
            idx_t k = j + 2 - 3 * m;      /* exponent difference */
            if (k > m) continue;          /* binomial zero */
            seq_t term = cache[m] * binom_small(m, k);
            if (k & 1) term = -term;      /* multiply by (−1)^k */
            sum += term;
        }
        cache[j] = sum;
    }

    hmax = n;
    return cache[n];
}
