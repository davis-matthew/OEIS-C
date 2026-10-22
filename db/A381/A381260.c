/*
 * A381260  Expansion of e.g.f. exp( -LambertW(-2 * x * cosh(x)) / 2 ).
 * https://oeis.org/A381260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th term of the sequence with e.g.f. A(x) = exp( x
 *            * cosh(x) * A(x)^2 ), a(0) = 1. The following implementation
 *            builds the sequence using the recurrence derived from this
 *            functional equation
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* static cache for computed values */
    static seq_t *a_cache = NULL;
    static seq_t *e_cache = NULL;
    static seq_t *c_cache = NULL;
    static idx_t cache_size = 0;     /* number of allocated slots */
    static idx_t max_computed = 0;   /* highest index computed so far */
    static bool initialized = false;

    /* initialise with term a(0)=1, e(0)=1 (a0*a0), c(0)=0 (unused) */
    if (!initialized) {
        cache_size = 4;   /* initial capacity, enough for a few terms */
        a_cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        e_cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        c_cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        a_cache[0] = (seq_t)1;
        e_cache[0] = (seq_t)1;   /* a0 * a0 */
        c_cache[0] = (seq_t)0;   /* unused */
        max_computed = 0;
        initialized = true;
    }

    /* enlarge cache if necessary */
    if (n + 2 > cache_size) {   /* reserve space for indices up to n+1 */
        idx_t new_cap = n + 4;
        a_cache = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        e_cache = (seq_t*)realloc(e_cache, new_cap * sizeof(seq_t));
        c_cache = (seq_t*)realloc(c_cache, new_cap * sizeof(seq_t));
        cache_size = new_cap;
    }

    /* compute values up to n */
    for (idx_t m = max_computed; m < n; ++m) {
        /* ------- compute c[m+1] -------- */
        idx_t M = m + 1;
        seq_t c_val = (seq_t)0;
        seq_t binom_c = (seq_t)1;   /* binomial(M,0) */
        for (idx_t j = 0; j <= M; ++j) {
            if (j & 1) { /* d_j = j for odd j, 0 for even j */
                seq_t term = binom_c * (seq_t)j * e_cache[M - j];
                c_val += term;
            }
            if (j < M) {
                binom_c = (binom_c * (seq_t)(M - j)) / (seq_t)(j + 1);
            }
        }
        c_cache[M] = c_val;

        /* ------- compute a[m+1] -------- */
        idx_t N = m;    /* index for a[m+1] recurrence uses n=m */
        seq_t a_val = (seq_t)0;
        seq_t binom_a = (seq_t)1;   /* binomial(N,0) */
        for (idx_t k = 0; k <= N; ++k) {
            seq_t term = binom_a * a_cache[k];
            idx_t idx = N - k + 1;  /* n-k+1 */
            term *= c_cache[idx];
            a_val += term;
            if (k < N) {
                binom_a = (binom_a * (seq_t)(N - k)) / (seq_t)(k + 1);
            }
        }
        a_cache[m + 1] = a_val;

        /* ------- compute e[m+1] -------- */
        idx_t L = m + 1;
        seq_t e_val = (seq_t)0;
        seq_t binom_e = (seq_t)1;   /* binomial(L,0) */
        for (idx_t i = 0; i <= L; ++i) {
            seq_t term = binom_e * a_cache[i] * a_cache[L - i];
            e_val += term;
            if (i < L) {
                binom_e = (binom_e * (seq_t)(L - i)) / (seq_t)(i + 1);
            }
        }
        e_cache[m + 1] = e_val;
    }

    max_computed = n;  /* new maximum computed */
    return a_cache[n];
}
