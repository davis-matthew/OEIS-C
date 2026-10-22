/*
 * A379878  E.g.f. A(x) satisfies A(x) = exp(-x) + x*A(x)^3.
 * https://oeis.org/A379878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^n + n * Σ_{i+j+k = n-1} (n-1)!/(i! j! k!) · a_i ·
 *            a_j · a_k This follows directly from A(x) = exp(−x) + x·A(x)^3,
 *            where A(x)=Σ a_k x^k/k!. The implementation caches previously
 *            computed values so that calls to a(idx_t n) are O(n^2) in total
 *            with n up to ~10 000
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#if defined(__cplusplus)
#  define SEQ_ALLOC(cnt)  new seq_t[(cnt)]
#  define SEQ_FREE(ptr)   delete [] (ptr)
#else
#  define SEQ_ALLOC(cnt)  ((seq_t*)malloc((cnt)*sizeof(seq_t)))
#  define SEQ_FREE(ptr)   free(ptr)
#endif

static seq_t *seq_a      = NULL;   /* a[] values */
static seq_t *seq_d      = NULL;   /* d[] = Σ a_i a_{m-i} * C(m,i)  */
static seq_t *seq_fact   = NULL;   /* factorials */
static size_t  seq_cap   = 0;       /* allocated capacity */
static idx_t   seq_pre   = 0;       /* largest n computed (0‑based) */

/* Ensure the static arrays can hold values up to 'need' (inclusive). */
static void ensure_capacity(idx_t need)
{
    if (need < seq_cap) return;

    size_t new_cap = (size_t)need + 1;
    seq_t *new_a   = SEQ_ALLOC(new_cap);
    seq_t *new_d   = SEQ_ALLOC(new_cap);
    seq_t *new_fac = SEQ_ALLOC(new_cap);

    /* copy existing data */
    for (size_t i = 0; i < seq_cap; ++i) {
        new_a[i]   = seq_a[i];
        new_d[i]   = seq_d[i];
        new_fac[i] = seq_fact[i];
    }

    /* free old storage */
    SEQ_FREE(seq_a);
    SEQ_FREE(seq_d);
    SEQ_FREE(seq_fact);

    seq_a    = new_a;
    seq_d    = new_d;
    seq_fact = new_fac;
    seq_cap  = new_cap;
}

/* Compute the sequence up to index 'n' if not already done. */
static void precompute(idx_t n)
{
    if (n <= seq_pre) return;          /* already computed */

    ensure_capacity(n);

    /* Compute missing factorials */
    for (size_t i = seq_cap; i <= (size_t)n; ++i) {
        if (i == 0) seq_fact[0] = (seq_t)1;
        else        seq_fact[i] = seq_fact[i-1] * (seq_t)i;
    }

    /* Initialize a[0] = 1, d[0] = 1 */
    seq_a[0] = (seq_t)1;
    seq_d[0] = (seq_t)1;

    /* Compute terms from seq_pre+1 up to n. */
    for (idx_t m = seq_pre + 1; m <= n; ++m) {
        /* -------- Compute b_{m-1} -------- */
        idx_t prev = m - 1;
        seq_t g    = (seq_t)0;               /* accumulator for b_{prev} */

        seq_t binom = (seq_t)1;              /* binom(prev,0) = 1 */
        for (idx_t k = 0; k <= prev; ++k) {
            seq_t term = seq_a[k] * seq_d[prev - k] * binom;
            g += term;

            /* Update binomial coefficient to binom(prev, k+1) */
            if (k < prev) {
                binom = binom * (seq_t)(prev - k) / (seq_t)(k + 1);
            }
        }

        /* -------- Compute a[m] -------- */
        seq_t tmp = (seq_t)m * g;             /* n * b_{n-1}  */
        seq_t a_m;
        if ((m & 1) == 0) {                  /* even: (-1)^n = +1 */
            a_m = tmp + (seq_t)1;
        } else {                             /* odd : (-1)^n = -1 */
            a_m = tmp - (seq_t)1;
        }
        seq_a[m] = a_m;

        /* -------- Compute d[m] -------- */
        seq_t sum = (seq_t)0;
        seq_t bin  = (seq_t)1;               /* binom(m,0) */
        for (idx_t i = 0; i <= m; ++i) {
            seq_t prod = seq_a[i] * seq_a[m - i] * bin;
            sum += prod;

            if (i < m) {
                bin = bin * (seq_t)(m - i) / (seq_t)(i + 1);
            }
        }
        seq_d[m] = sum;
    }

    seq_pre = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* a(0) defined explicitly */

    precompute(n);
    return seq_a[n];
}
