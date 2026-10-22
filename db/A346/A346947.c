/*
 * A346947  Expansion of e.g.f. log( 1 + log(1 + x)^5 / 5! ).
 * https://oeis.org/A346947
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Stirling1(n,5) - (1/n) * Sum_{k=1..n-1} binomial(n,k) *
 *            Stirling1(n-k,5) * k * a(k) Initial values: a(5)=1, a(6)=-15,
 *            a(7)=175, a(8)=-1960, a(9)=22449 (computed via the recurrence
 *            above)
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 13, i64 n <= 20, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* External integer types provided by the build */
typedef uint64_t idx_t;

/* Function prototype for seq_t (signed integer type)        */
/* Defined externally; do not redeclare it. */
extern seq_t a(idx_t n);

/* Internal helper: compute values up to index n (inclusive) */
static seq_t *a_cache = NULL;     /* cache of a(n) */
static seq_t *b_cache = NULL;     /* cache of Stirling1(n,5) */
static size_t a_cap   = 0, b_cap = 0;   /* allocated capacities */
static idx_t last_computed = 0;          /* last index fully computed */

/* current row of signed Stirling numbers of the first kind (k=0..5) */
static seq_t stirling_row[6] = {1,0,0,0,0,0};   /* S(0,0)=1, others 0 */

/* Ensure that a_cache, b_cache are allocated up to index n, and
   all needed values up to n have been computed. */
static void ensure_up_to(idx_t n)
{
    if (n <= last_computed) return;

    /* Allocate or grow caches as necessary */
    if (a_cache == NULL) {
        a_cap = 128;
        a_cache = (seq_t*)malloc(a_cap * sizeof(seq_t));
        b_cache = (seq_t*)malloc(a_cap * sizeof(seq_t));
        a_cache[0] = 0;
        b_cache[0] = 0;
    }
    if ((size_t)(n + 1) > a_cap) {
        size_t new_cap = a_cap;
        while ((size_t)(n + 1) > new_cap) new_cap <<= 1;
        a_cache = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        b_cache = (seq_t*)realloc(b_cache, new_cap * sizeof(seq_t));
        a_cap = b_cap = new_cap;
    }

    /* Compute all values from last_computed+1 up to n */
    for (idx_t cur = last_computed + 1; cur <= n; ++cur) {
        /* Update Stirling numbers for row cur using
           S(n,k) = S(n-1,k-1) - (n-1)*S(n-1,k)   (k>=1)  */
        for (int k = 5; k >= 1; --k) {
            stirling_row[k] = stirling_row[k-1] - (seq_t)(cur - 1) * stirling_row[k];
        }
        stirling_row[0] = 0;  /* for cur > 0 */
        b_cache[cur] = stirling_row[5];   /* S(cur,5) */

        /* Compute a(cur) via the recurrence */
        if (cur < 5) {
            a_cache[cur] = 0;
            continue;
        }

        seq_t sum = 0;          /* Σ binom(cur,k) * S(cur-k,5) * k * a(k) */
        seq_t binom = 1;        /* binom(cur,0) */

        for (idx_t k = 1; k < cur; ++k) {
            /* binom(cur,k) = binom(cur,k-1) * (cur-k+1) / k   (exact) */
            binom = binom * ((seq_t)(cur - k + 1));
            binom = binom / k;

            /* term = binom * S(cur-k,5) * k * a(k)  */
            seq_t term = binom * b_cache[cur - k];
            term = term * ((seq_t)k * a_cache[k]);

            sum += term;
        }

        /* Final recurrence: a(cur) = S(cur,5) - sum / cur */
        a_cache[cur] = b_cache[cur] - (sum / (seq_t)cur);
    }

    last_computed = n;
}

/* Public entry point */
seq_t a(idx_t n)
{
    if (n < 5) return 0;          /* values below the offset are irrelevant */
    ensure_up_to(n);
    return a_cache[n];
}
