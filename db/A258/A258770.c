/*
 * A258770  Expansion of f(-x^2)^4 * psi(-x^3) in powers of x where psi(), f()
 *          are Ramanujan theta functions.
 * https://oeis.org/A258770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 164, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A258770:  a(n) = coefficient of x^n in
   ∏_{k>0} (1 - x^(2k))^4 * (1 - x^(3k)) * (1 + x^(6k))
*/

#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Allocation helpers – work for both C and C++.  For C++ seq_t may be a
   class type (e.g., cpp_int).  For C the allocations are simple.
*/
#ifdef __cplusplus
# define ALLOC_SEQ(n)  new seq_t[(size_t)(n)]
# define FREE_SEQ(p)   delete [] (p)
#else
# define ALLOC_SEQ(n)  ((seq_t*)calloc((size_t)(n), sizeof(seq_t)))
# define FREE_SEQ(p)   free((p))
#endif

/* ------------------------------------------------------------------ */
/* Binomial coefficients for (1 - x^p)^4 */
static const int binom4[5] = {1, 4, 6, 4, 1};

/* ------------------------------------------------------------------ */
/* Persistent storage for coefficients up to the largest requested n */
static seq_t *coeff  = NULL;    /* array a[0..maxN]   */
static size_t coeff_cap = 0;    /* number of allocated elements */

/* ------------------------------------------------------------------ */
/* Compute all coefficients a[0..N] (linear‑time in N^2)          */
static void compute_up_to(size_t N)
{
    /* allocate two buffers of length N+1 */
    seq_t *a  = ALLOC_SEQ(N + 1);
    seq_t *b  = ALLOC_SEQ(N + 1);
    if (!a || !b) return;                /* allocation failure */

    /* initialise first buffer */
    for (size_t i = 0; i <= N; ++i) a[i] = 0;
    a[0] = 1;   /* constant term */

    /* for each k = 1..N apply the three factors */
    for (size_t k = 1; k <= N; ++k) {
        /* factor (1 - x^(2k))^4  ->  5 terms, alternating signs  */
        for (size_t i = 0; i <= N; ++i) b[i] = 0;
        for (size_t j = 0; j <= N; ++j) {
            seq_t val = a[j];
            if (val == 0) continue;
            for (int t = 0; t <= 4; ++t) {
                size_t idx = j + (size_t)(2 * k) * (size_t)t;
                if (idx > N) break;
                seq_t term = val * (seq_t)binom4[t];
                if (t & 1) term = -term;          /* (-1)^t */
                b[idx] += term;
            }
        }
        seq_t *tmp = a; a = b; b = tmp;          /* swap buffers */

        /* factor (1 - x^(3k))  ->  2 terms, sign alternates  */
        for (size_t i = 0; i <= N; ++i) b[i] = 0;
        for (size_t j = 0; j <= N; ++j) {
            seq_t val = a[j];
            if (val == 0) continue;
            /* t = 0  : +x^0  term */
            b[j] += val;
            /* t = 1  : -x^(3k) term */
            size_t idx = j + (size_t)(3 * k);
            if (idx <= N) b[idx] -= val;
        }
        tmp = a; a = b; b = tmp;

        /* factor (1 + x^(6k))  ->  2 terms, both positive  */
        for (size_t i = 0; i <= N; ++i) b[i] = 0;
        for (size_t j = 0; j <= N; ++j) {
            seq_t val = a[j];
            if (val == 0) continue;
            b[j] += val;                     /* t = 0  */
            size_t idx = j + (size_t)(6 * k);
            if (idx <= N) b[idx] += val;      /* t = 1  */
        }
        tmp = a; a = b; b = tmp;
    }

    /* free the old coefficient array, keep the new one */
    if (coeff) FREE_SEQ(coeff);
    coeff      = a;          /* a now holds the final coefficients */
    coeff_cap  = N + 1;
    FREE_SEQ(b);             /* b is now unused */
}

/* ------------------------------------------------------------------ */
/* Public interface – required by the test harness                 */
seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    if (N >= coeff_cap) compute_up_to(N);
    return coeff[N];
}

/* ------------------------------------------------------------------ */
