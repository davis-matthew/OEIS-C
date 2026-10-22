/*
 * A364646  G.f. satisfies A(x) = 1/(1 - 4*x) - x*A(x)^3.
 * https://oeis.org/A364646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A364646
   Recurrence:  a(0) = 1
                a(n) = 4^n - Σ_{i+j+k=n-1} a(i)a(j)a(k)   (n ≥ 1)
   This implementation keeps a cache of all computed terms and a
   convolution array c[m] = Σ_{i=0}^{m} a(i)a(m-i) to evaluate the
   triple sum in O(n) time for each additional term.  It is fully
   compatible with the C11 and C++17 build environments used for the
   OEIS evaluation harness. */

/* minimal includes needed for dynamic allocation and memset */
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

/* -------------  sequence generator function  ------------- */
seq_t a(idx_t n) {
    /* static cache state ---------------------------------- */
    static size_t max_n = 0;          /* largest n already computed */
    static seq_t *a_arr  = NULL;      /* a[0..max_n]                     */
    static seq_t *c_arr  = NULL;      /* c[0..2*max_n]                   */
    static size_t cap_a = 0;          /* current capacity of a_arr        */
    static size_t cap_c = 0;          /* current capacity of c_arr        */
    static seq_t pow4 = 1;            /* 4^max_n                          */

    size_t N = (size_t)n;             /* convert to host size_t          */

    /* ----------------------------------------------------- */
    /* Ensure storage for a[0..N] ************************** */
    if (cap_a < N + 1) {
        size_t new_cap = cap_a ? cap_a << 1 : 1;
        while (new_cap < N + 1) new_cap <<= 1;
        a_arr = (seq_t*)realloc(a_arr, new_cap * sizeof(seq_t));
        cap_a = new_cap;
    }

    /* Ensure storage for c[0..2N] (indicies up to 2*N)  */
    size_t required_c = 2 * N + 1;
    if (cap_c < required_c) {
        size_t old_cap = cap_c;
        size_t new_cap = cap_c ? cap_c << 1 : 1;
        while (new_cap < required_c) new_cap <<= 1;
        c_arr = (seq_t*)realloc(c_arr, new_cap * sizeof(seq_t));
        /* zero newly allocated part of c_arr */
        if (new_cap > old_cap)
            memset((void*)(c_arr + old_cap),
                   0,
                   (new_cap - old_cap) * sizeof(seq_t));
        cap_c = new_cap;
    }

    /* First call: fill base cases ------------------------ */
    if (max_n == 0 && c_arr != NULL && a_arr != NULL) {
        /* a[0] = 1 */
        a_arr[0] = (seq_t)1;
        /* convolution for index 0: a[0]*a[0] */
        c_arr[0] = a_arr[0] * a_arr[0];
    }

    /* Compute new terms up to N -------------------------- */
    for (size_t m = max_n + 1; m <= N; ++m) {
        /* update 4^m */
        pow4 *= (seq_t)4;

        /* compute triple sum t = Σ_{k=0}^{m-1} a(k) * c(m-1-k) */
        seq_t t = 0;
        for (size_t k = 0; k < m; ++k)
            t += a_arr[k] * c_arr[(m - 1) - k];

        /* a[m] = 4^m - t */
        a_arr[m] = pow4 - t;

        /* update convolution array c */
        /* contributions with new term a[m] */
        for (size_t i = 0; i < m; ++i) {
            seq_t prod = a_arr[i] * a_arr[m];
            prod += prod;                     /* multiply by 2 */
            c_arr[i + m] += prod;
        }
        /* contribution of pair (m,m) */
        c_arr[2 * m] += a_arr[m] * a_arr[m];
    }

    max_n = N;
    return a_arr[N];
}
