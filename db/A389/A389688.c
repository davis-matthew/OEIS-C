/*
 * A389688  Expansion of e.g.f. exp(x*cosh(x) + x^2*sinh(x)/2).
 * https://oeis.org/A389688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n≥1: a(n) = Σ_{odd k=1..n} (k*(k+1)/2) * C(n-1,
 *            k-1) * a(n-k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *A_cache = NULL;        /* memoised a(n) values */
static size_t A_cap   = 0;           /* current allocated capacity */
static idx_t   A_last  = 0;           /* largest n whose a(n) has been computed */

/* Ensure that a(0)…a(n) are stored in A_cache. */
static void ensure(idx_t n)
{
    /* Allocate space for a(0) on first use */
    if (A_cache == NULL) {
        A_cap   = 1;
        A_cache = (seq_t *)realloc(A_cache, A_cap * sizeof(seq_t));
        if (A_cache == NULL) abort();
        A_cache[0] = (seq_t)1;
        A_last     = 0;
    }

    if (n <= A_last) return;          /* already available */

    /* Grow the array if necessary */
    size_t new_cap = A_cap;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    if (new_cap > A_cap) {
        seq_t *tmp = (seq_t *)realloc(A_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        A_cache = tmp;
        A_cap   = new_cap;
    }

    /* Compute new entries up to n */
    for (idx_t i = A_last + 1; i <= n; ++i) {
        seq_t sum   = (seq_t)0;
        idx_t N     = i - 1;                 /* N = i-1 for binomials */
        seq_t comb  = (seq_t)1;              /* C(N, 0) */

        for (idx_t s = 1; s <= i; s += 2) {
            /* weight of a block of size s: w = s*(s+1)/2 */
            seq_t w = ((seq_t)s * (s + 1)) / (seq_t)2;

            /* term = C(N, s-1) * w * a(i-s) */
            seq_t term = comb;
            term *= w;
            term *= A_cache[i - s];
            sum += term;

            /* update comb for next odd block size s+2, if needed */
            if (s + 2 <= i) {
                seq_t tmp = comb;
                tmp *= (seq_t)(N - (s - 1));
                tmp *= (seq_t)(N - (s - 1) - 1);
                tmp /= (seq_t)s;
                tmp /= (seq_t)(s + 1);
                comb = tmp;
            }
        }

        A_cache[i] = sum;
    }
    A_last = n;
}

seq_t a(idx_t n)
{
    ensure(n);
    return A_cache[n];
}
