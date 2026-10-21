/*
 * A255933  a(n) is the largest integer m such that s/(m!-1) is an integer,
 *          where s is the sum of all previous terms; a(1)=1.
 * https://oeis.org/A255933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = largest integer m such that (sum_{i=1}^{n-1} a(i)) / (m!-1) is an integer;
    a(1) = 1.  This implementation memoises previously computed terms
    and computes each a(n) by iterating m from 2 upwards until (m!-1)
    exceeds the current partial sum. */

#include <stdlib.h>

static seq_t *terms = NULL;   /* a(i) for i >= 1, stored from index 1 */
static seq_t *cum   = NULL;   /* cumulative sum, cum[i] = sum_{1..i} */
static size_t cap = 0;        /* array capacity (>= needed indices + 1) */
static idx_t len = 0;         /* largest i for which terms[i] is defined */

/* Ensure that the arrays have room for indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n <= cap)
        return;
    size_t newcap = (cap == 0) ? 8 : cap << 1;
    while (newcap < (size_t)n)
        newcap <<= 1;
    seq_t *t = (seq_t *)realloc(terms, (newcap + 1) * sizeof(seq_t));
    seq_t *c = (seq_t *)realloc(cum,   (newcap + 1) * sizeof(seq_t));
    if (!t || !c) {
        /* Abort on allocation failure. */
        abort();
    }
    terms = t;
    cum   = c;
    cap   = newcap;
}

/* Compute a(n).  The function is safe to call in any order of n. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; n=0 is undefined. */
        return (seq_t)0;

    /* Initialise storage on first call. */
    if (len == 0) {
        ensure_capacity((idx_t)1);
        cum[0] = (seq_t)0;
        terms[1] = (seq_t)1;
        cum[1]   = terms[1];
        len = 1;
    }

    /* Compute all terms up to n if needed. */
    if (n > len) {
        ensure_capacity(n);
        for (idx_t i = len + 1; i <= n; ++i) {
            seq_t sumPrev = cum[i - 1];
            seq_t best = (seq_t)1;          /* at least 1 (by definition) */
            seq_t fac = (seq_t)1;           /* 1! */
            for (idx_t m = 2;; ++m) {
                fac *= (seq_t)m;            /* factorial */
                seq_t d = fac - (seq_t)1;   /* m! - 1 */
                if (d > sumPrev)
                    break;
                if (sumPrev % d == (seq_t)0)
                    best = (seq_t)m;
            }
            terms[i] = best;
            cum[i]   = cum[i - 1] + best;
        }
        len = n;
    }

    return terms[n];
}
