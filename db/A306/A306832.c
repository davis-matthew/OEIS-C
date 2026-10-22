/*
 * A306832  Expansion of e.g.f. (sec(x) + tan(x))*exp(x)*BesselI(0,2*x).
 * https://oeis.org/A306832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=0}^{n} binom(n,i) * E_i * S_{n-i} where E_i are
 *            Euler zigzag numbers (sec(x)+tan(x)) coefficients and S_k are
 *            the coefficients of exp(x)*I0(2x); S_k = sum_{j=0}^{⌊k/2⌋}
 *            binom(k,2j)*binom(2j,j) (central trinomial C_{j-1}). See also:
 *            A306832
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* ensure that *ptr has at least 'needed' elements.
   new elements are zeroed. */
static void ensure_capacity(size_t needed, seq_t **ptr, size_t *cap)
{
    if (needed <= *cap)
        return;
    size_t new_cap = needed * 2 + 16;
    seq_t *new_arr = (seq_t *)calloc(new_cap, sizeof(seq_t));
    if (*ptr) {
        memcpy(new_arr, *ptr, (*cap) * sizeof(seq_t));
        free(*ptr);
    }
    *ptr = new_arr;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* static state */
    static idx_t max_calculated = 0;
    static seq_t *E = NULL, *S = NULL, *A = NULL;
    static seq_t *D = NULL;          /* binom(2k,k) */
    static size_t e_cap = 0, s_cap = 0, a_cap = 0, d_cap = 0;
    static size_t d_len = 0;          /* number of D entries computed */

    /* initialise once */
    if (E == NULL) {
        ensure_capacity(1, &E, &e_cap);
        ensure_capacity(1, &S, &s_cap);
        ensure_capacity(1, &A, &a_cap);
        ensure_capacity(1, &D, &d_cap);

        E[0] = (seq_t)1;
        S[0] = (seq_t)1;
        A[0] = (seq_t)1;          /* a(0) = 1 */
        D[0] = (seq_t)1;          /* binom(0,0) = 1 */
        d_len = 1;
        max_calculated = 0;
    }

    if (n == 0)
        return A[0];

    /* expand tables up to n */
    if (n > max_calculated) {
        /* ensure tables can hold n+1 elements */
        ensure_capacity((size_t)n + 1, &E, &e_cap);
        ensure_capacity((size_t)n + 1, &S, &s_cap);
        ensure_capacity((size_t)n + 1, &A, &a_cap);

        /* compute new values for E and S and build D on the fly */
        for (idx_t i = max_calculated; i < n; ++i) {
            /* 1. Euler numbers E_{i+1} */
            if (i == 0) {
                E[1] = (seq_t)1;
            } else {
                seq_t sum = (seq_t)0;
                seq_t binomV = (seq_t)1;            /* binom(i,0) */
                for (idx_t k = 0; k <= i; ++k) {
                    sum += binomV * E[(size_t)k] * E[(size_t)(i - k)];
                    if (k < i)
                        binomV = binomV * (i - k) / (k + 1);
                }
                E[i + 1] = sum / (seq_t)2;          /* division is exact */
            }

            /* 2. pre‑compute D[0..(i+1)/2] if needed */
            idx_t maxK = (i + 1) / 2;
            if ((size_t)maxK + 1 > d_cap) {
                ensure_capacity((size_t)maxK + 1, &D, &d_cap);
            }
            for (idx_t cur = d_len; cur <= maxK; ++cur) {
                seq_t prev = D[cur - 1];
                seq_t mult = (seq_t)(2 * cur) * (seq_t)(2 * cur - 1);
                seq_t denom = (seq_t)cur * (seq_t)cur;
                D[cur] = (prev * mult) / denom;
            }
            if ((size_t)maxK + 1 > d_len)
                d_len = (size_t)maxK + 1;

            /* 3. S_{i+1} = sum_{j=0}^{i+1} binom(i+1,j)*[j even]*D[j/2] */
            seq_t sumS = (seq_t)0;
            seq_t binomV = (seq_t)1;            /* binom(i+1,0) */
            for (idx_t j = 0; j <= i + 1; ++j) {
                if ((j & 1) == 0) {
                    idx_t k = j >> 1;
                    sumS += binomV * D[k];
                }
                if (j < (i + 1))
                    binomV = binomV * (i + 1 - j) / (j + 1);
            }
            S[i + 1] = sumS;
        }

        /* 4. compute A[m] for new indices */
        for (idx_t m = max_calculated + 1; m <= n; ++m) {
            seq_t sumA = (seq_t)0;
            seq_t binomV = (seq_t)1;            /* binom(m,0) */
            for (idx_t i = 0; i <= m; ++i) {
                sumA += binomV * E[(size_t)i] * S[(size_t)(m - i)];
                if (i < m)
                    binomV = binomV * (m - i) / (i + 1);
            }
            A[m] = sumA;
        }

        max_calculated = n;
    }

    return A[n];
}
