/*
 * A267172  Growth series for affine Coxeter group B_9.
 * https://oeis.org/A267172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coeff of (Π_i (1-x^{m_i+1})) / ((1-x)^k * Π_i
 *            (1-x^{m_i})), where k = 9 and m_i = 1,3,5,7,9,11,13,15,17
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 47, u64 n <= 780, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Coefficients of the denominator D(x) = (1-x)^9 * Π_i (1-x^{m_i})   */
/* Length 91 → coefficients for degrees 0 … 90 */
static signed long long Bcoeff[91];

/* Coefficients of the numerator N(x) = Π_i (1-x^{m_i+1}) */
/* Length 91 → degrees 0 … 90   */
static signed long long Acoeff[91];

static void init_coeffs(void)
{
    const int m[9] = {1,3,5,7,9,11,13,15,17};
    /* Build numerator Acoeff */
    signed long long tmp[91];
    for (int i = 0; i < 91; ++i) tmp[i] = 0;
    tmp[0] = 1;                /* start with constant 1   */
    int curdeg = 0;
    for (int idx = 0; idx < 9; ++idx) {
        int e = m[idx] + 1;    /* exponent of the factor (1-x^{m+1}) */
        signed long long newtmp[91] = {0};
        for (int i = 0; i <= curdeg; ++i) {
            newtmp[i]     += tmp[i];
            newtmp[i + e] -= tmp[i];
        }
        curdeg += e;
        for (int i = 0; i <= curdeg; ++i) tmp[i] = newtmp[i];
    }
    for (int i = 0; i < 91; ++i) Acoeff[i] = tmp[i];

    /* Build denominator Bcoeff: (1-x)^9 * Π (1-x^{m}) */
    signed long long temp[91];
    for (int i = 0; i < 91; ++i) temp[i] = 0;
    temp[0] = 1;                /* start with constant 1   */
    curdeg = 0;
    /* first factor (1-x)^9  */
    for (int t = 0; t < 9; ++t) {
        signed long long newtmp[91] = {0};
        for (int i = 0; i <= curdeg; ++i) {
            newtmp[i]     += temp[i];
            newtmp[i + 1] -= temp[i];
        }
        curdeg += 1;
        for (int i = 0; i <= curdeg; ++i) temp[i] = newtmp[i];
    }
    /* multiply by each (1-x^m) */
    for (int idx = 0; idx < 9; ++idx) {
        int e = m[idx];
        signed long long newtmp[91] = {0};
        for (int i = 0; i <= curdeg; ++i) {
            newtmp[i]     += temp[i];
            newtmp[i + e] -= temp[i];
        }
        curdeg += e;
        for (int i = 0; i <= curdeg; ++i) temp[i] = newtmp[i];
    }
    for (int i = 0; i < 91; ++i) Bcoeff[i] = temp[i];
}

/* Cache of computed terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;

/* compute the nth term of the sequence   */
seq_t a(idx_t n)
{
    /* initialize constants on first use */
    static int initialized = 0;
    if (!initialized) {
        init_coeffs();
        initialized = 1;
    }

    /* extend cache if necessary */
    if ((idx_t)cache_size <= n) {
        size_t newsize = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) {                 /* memory failure – safe default */
            return 0;
        }
        cache = tmp;

        for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
            seq_t pos_sum = 0;
            seq_t neg_sum = 0;

            int maxj = (i < 90) ? (int)i : 90;   /* denominator degree 90 */

            for (int j = 1; j <= maxj; ++j) {
                signed long long bj = Bcoeff[j];
                if (bj > 0) {
                    pos_sum += cache[i - (idx_t)j] * (unsigned)bj;
                } else if (bj < 0) {
                    neg_sum += cache[i - (idx_t)j] * (unsigned)(-bj);
                }
            }

            /* contribution of the numerator N(x)   */
            int aneg = (i <= 90) ? Acoeff[i] : 0;

            seq_t term = neg_sum;
            if (aneg >= 0) {
                term += (seq_t)aneg;
            } else {
                term -= (seq_t)(-aneg);
            }
            term -= pos_sum;
            cache[i] = term;
        }
        cache_size = newsize;
    }
    return cache[n];
}
