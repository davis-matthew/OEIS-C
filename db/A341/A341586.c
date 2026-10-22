/*
 * A341586  E.g.f.: (exp(1 - exp(x)) - 1)^2 / 2.
 * https://oeis.org/A341586
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k=2..n} (-1)^k * Stirling2(n,k) * (2^(k-1) - 1)
 * Range:     i8 n <= 8, i16 n <= 11, i32 n <= 17, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static state for memoization */
static seq_t *a_cache      = NULL;   /* cache for a(n) values */
static size_t a_cache_size = 0;      /* allocated size of a_cache */
static idx_t  max_computed  = 0;      /* largest n computed so far */

static seq_t *S_prev = NULL;         /* previous Stirling row */
static size_t S_prev_size = 0;       /* allocated size of S_prev */

/* initialise static buffers for n = 1 (row with S(1,1)=1) */
static void init_static(void) {
    a_cache_size = 3;                           /* indices 0..2 */
    a_cache      = (seq_t*)calloc(a_cache_size, sizeof(seq_t)); /* zeros */

    S_prev_size = 2;                           /* indices 0..1 */
    S_prev      = (seq_t*)calloc(S_prev_size, sizeof(seq_t));
    S_prev[1] = (seq_t)1;                     /* S(1,1)=1 */

    max_computed = 1;
}

/* compute a(n) for n >= 2, memoising computed values */
seq_t a(idx_t n) {
    if (max_computed == 0) init_static();      /* first call */

    if (n <= 1) return (seq_t)0;               /* below offset */

    /* extend precomputation up to requested n */
    if (n > max_computed) {
        /* enlarge a_cache if necessary */
        if (n + 1 > a_cache_size) {
            size_t new_size = n + 1;
            seq_t *tmp = (seq_t*)realloc(a_cache, new_size * sizeof(seq_t));
            a_cache = tmp;
            /* zero initialise new elements */
            for (size_t i = a_cache_size; i < new_size; ++i) a_cache[i] = (seq_t)0;
            a_cache_size = new_size;
        }

        /* compute rows from max_computed+1 up to n */
        for (idx_t curr = max_computed + 1; curr <= n; ++curr) {
            /* allocate current row */
            seq_t *S_curr = (seq_t*)calloc((size_t)curr + 1, sizeof(seq_t));
            /* S_curr[0] is already zero */

            /* Stirling recurrence S(n,k) = k*S(n-1,k) + S(n-1,k-1) */
            for (idx_t k = 1; k <= curr; ++k) {
                seq_t term1 = (k <= curr - 1) ? (seq_t)k * S_prev[k] : (seq_t)0;
                seq_t term2 = S_prev[k - 1];
                S_curr[k] = term1 + term2;
            }

            /* compute a(curr) from the defining sum */
            seq_t sum = (seq_t)0;
            seq_t pow2 = (seq_t)2;      /* 2^(k-1) for k=2 */
            for (idx_t k = 2; k <= curr; ++k) {
                seq_t mult = pow2 - (seq_t)1;    /* 2^(k-1)-1 */
                seq_t term = S_curr[k];
                if (k & 1) term = -term;          /* (-1)^k */
                term *= mult;
                sum += term;
                pow2 *= (seq_t)2;                 /* advance power of two */
            }
            a_cache[curr] = sum;

            /* recycle memory: discard previous row, keep current */
            free(S_prev);
            S_prev      = S_curr;
            S_prev_size = (size_t)curr + 1;
        }
        max_computed = n;
    }

    return a_cache[n];
}
