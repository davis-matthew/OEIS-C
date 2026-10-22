/*
 * A349046  G.f. A(x) satisfies: A(x) = 1 / (1 + x - 2 * x * A(-4*x)).
 * https://oeis.org/A349046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(n) = -a(n-1) + 2 * Σ_{k=0}^{n-1} (-4)^k * a(k) *
 *            a(n-k-1) (n ≥ 1)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 5, i64 n <= 8, i128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_COMPUTE 10001   /* safe upper bound for the required range (≈10 000) */

static seq_t  terms[MAX_COMPUTE];   /* computed sequence values */
static seq_t  powm4[MAX_COMPUTE];   /* (-4)^k values */
static idx_t  computed = 0;         /* number of terms already computed ( ≥ 1 if computed ≥ 1 ) */

seq_t a(idx_t n)
{
    /* initialize static arrays on first use ----------------------------------------------------- */
    if (computed == 0 && n < MAX_COMPUTE) {
        terms[0] = (seq_t)1;            /* a(0) = 1 */
        powm4[0] = (seq_t)1;            /* (-4)^0 = 1 */
        computed = 1;
    }

    /* compute up to requested n --------------------------------------------------------------- */
    if (n < computed) {      /* value already computed */
        return terms[n];
    }

    if (n >= MAX_COMPUTE) {
        /* safety check: if n exceeds the pre‑allocated buffer, fallback to dynamic growth */
        static seq_t *dyn_terms  = NULL;
        static seq_t *dyn_powm4  = NULL;
        static idx_t  dyn_size   = 0;

        if (dyn_size == 0) {
            dyn_size = n + 1;
            dyn_terms = (seq_t*)malloc(dyn_size * sizeof(seq_t));
            dyn_powm4 = (seq_t*)malloc(dyn_size * sizeof(seq_t));
            if (!dyn_terms || !dyn_powm4) {
                /* allocation failure – return 0 (undefined behaviour for huge n) */
                return (seq_t)0;
            }
            dyn_terms[0] = (seq_t)1;
            dyn_powm4[0] = (seq_t)1;
            for (idx_t i = 1; i <= n; ++i) {
                dyn_powm4[i] = dyn_powm4[i-1] * (seq_t)(-4);
                seq_t sum = (seq_t)0;
                for (idx_t k = 0; k < i; ++k) {
                    seq_t prod = dyn_terms[k] * dyn_terms[i-1-k];
                    sum += dyn_powm4[k] * prod;
                }
                dyn_terms[i] = -dyn_terms[i-1] + (seq_t)2 * sum;
            }
            return dyn_terms[n];
        }
    }

    for (idx_t m = computed; m <= n; ++m) {
        /* compute (-4)^m from the previous power */
        powm4[m] = powm4[m-1] * (seq_t)(-4);

        /* compute the convolution sum */
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < m; ++k) {
            seq_t prod = terms[k] * terms[m-1-k];
            sum += powm4[k] * prod;
        }

        /* recurrence: a(m) = -a(m-1) + 2 * sum */
        terms[m] = -terms[m-1] + (seq_t)2 * sum;
    }

    computed = n + 1;
    return terms[n];
}
