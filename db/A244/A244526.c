/*
 * A244526  Expansion of f(-x^3, -x^5)^2 in powers of x where f() is
 *          Ramanujan's two-variable theta function.
 * https://oeis.org/A244526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A244526: a(n) = coefficient of x^n in f(-x^3,-x^5)^2
 *  (Ramanujan's two-variable theta function, squared).
 *  The sequence is the convolution square of the sparse series
 *    s[m] = (-1)^k   if m = 4k^2 ∓ k   (k∈ℤ, k≠0 gives two exponents),
 *           0         otherwise.
 *  Thus  a(n) = Σ_{i=0..n} s[i] * s[n-i].
 */

#include <stdlib.h>
#include <string.h>

static seq_t *coeff = NULL;   /* buffer for s[0..] */
static size_t coeff_cap = 0;  /* current capacity of coeff */

seq_t a(idx_t n)
{
    /* work out needed array size */
    size_t sz = (size_t)n + 1;

    /* reallocate if necessary */
    if (sz > coeff_cap) {
#ifdef __cplusplus
        delete[] coeff;           /* delete old array in C++ */
#else
        free(coeff);              /* free old array in C */
#endif
        coeff_cap = sz;
#ifdef __cplusplus
        coeff = new seq_t[sz];    /* default‑construct in C++ */
#else
        coeff = (seq_t*)calloc(sz, sizeof(seq_t)); /* zeroed in C */
#endif
        if (!coeff) return 0;     /* allocation failure (unlikely) */
    }

    /* initialise all entries to 0 */
    for (size_t i = 0; i < sz; ++i)
        coeff[i] = 0;

    /* fill the sparse series s[m] */
    for (idx_t k = 0; ; ++k) {
        idx_t exp1 = 4 * k * k - k;          /* for k ≥ 0 */
        if (exp1 > n) break;
        seq_t sign = (k & 1 ? -1 : 1);      /* (-1)^k */
        coeff[(size_t)exp1] = sign;

        if (k > 0) {                         /* exponent for -k */
            idx_t exp2 = 4 * k * k + k;
            if (exp2 <= n)
                coeff[(size_t)exp2] = sign;
        }
    }

    /* convolution square a[n] = Σ s[i]·s[n-i] */
    seq_t res = 0;
    for (idx_t i = 0; i <= n; ++i) {
        seq_t vi = coeff[(size_t)i];
        if (vi != 0) {
            seq_t vj = coeff[(size_t)(n - i)];
            if (vj != 0)
                res += vi * vj;
        }
    }

    return res;
}
