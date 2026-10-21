/*
 * A294117  a(n) = (n!)^2 * Sum_{k=1..n} binomial(n,k) / k^2.
 * https://oeis.org/A294117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!)^2 * ∑_{k=1..n} binomial(n,k) / k^2 Recurrence used
 *            for O(n) computation (with initial terms a(1)=1, a(2)=9,
 *            a(3)=139, a(4)=3460): a(n) = (5n²‑7n+3)a(n‑1) –
 *            (n‑1)²(9n²‑24n+17)a(n‑2) + (n‑2)³(n‑1)²(7n‑13)a(n‑3) –
 *            2(n‑3)³(n‑2)³(n‑1)²a(n‑4) for n≥5
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache of computed terms */
static size_t cache_size = 0;    /* number of allocated slots (>= last_computed+1) */
static size_t last_computed = 0; /* highest index with a computed value */

/* Ensure that the cache can hold indices up to `n` (inclusive). */
static void ensure_cache_size(idx_t n)
{
    size_t needed = (size_t)n + 1;   /* indices [0..n] */
    if (needed <= cache_size)
        return;

    seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!tmp) abort();              /* out of memory – program aborts */
    cache = tmp;

    /* Zero‑initialize any newly allocated entries. */
    for (size_t i = cache_size; i < needed; ++i)
        cache[i] = 0;

    cache_size = needed;
}

seq_t a(idx_t n)
{
    if (n < 1)                     /* undefined for n<1; return 0 */
        return 0;

    ensure_cache_size(n);

    /* Compute any pending values up to `n`. */
    for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
        idx_t ii = (idx_t)i;

        if (ii == 1) {
            cache[i] = 1;          /* a(1) */
        } else if (ii == 2) {
            cache[i] = 9;          /* a(2) */
        } else if (ii == 3) {
            cache[i] = 139;        /* a(3) */
        } else if (ii == 4) {
            cache[i] = 3460;       /* a(4) */
        } else { /* ii >= 5, use recurrence */
            seq_t nseq   = (seq_t)ii;
            seq_t n1     = nseq - 1;
            seq_t n2     = nseq - 2;
            seq_t n3     = nseq - 3;

            /* Coefficients as per the recurrence. */
            seq_t coeff1 = (5 * nseq * nseq) - (7 * nseq) + 3;
            seq_t coeff2 = (n1 * n1) * ((9 * nseq * nseq) - (24 * nseq) + 17);
            seq_t coeff3 = (n2 * n2 * n2) * (n1 * n1) * ((7 * nseq) - 13);
            seq_t coeff4 = 2 * (n3 * n3 * n3) * (n2 * n2 * n2) * (n1 * n1);

            seq_t term = coeff1 * cache[i - 1];
            term -= coeff2 * cache[i - 2];
            term += coeff3 * cache[i - 3];
            term -= coeff4 * cache[i - 4];
            cache[i] = term;
        }
    }

    last_computed = n;
    return cache[n];
}
