/*
 * A347846  a(n) is the number of (strict) chains of subspaces with ends 0 and
 *          (F_9)^n.
 * https://oeis.org/A347846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n > 0: a(n) = 1 + Σ_{d=1}^{n-1} C_q(n,d) * a(n-d)
 *            where C_q(n,d) is the Gaussian binomial coefficient (n choose
 *            d)_q with q = 9 (C_q(n,d) = ∏_{i=0}^{d-1} (q^{n-i} – 1) /
 *            (q^{d-i} – 1) ) This counts chains of subspaces 0 < V_1 < … <
 *            V_k = (F₉)ⁿ
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h> /* size_t */

seq_t a(idx_t n) {
    /* static cache for a[0…max] */
    static seq_t *a_arr = NULL;
    static size_t a_size = 0;
    static idx_t a_max = 0;          /* highest n for which a_arr[n] is computed */

    /* cache for powers of q = 9 : qpow[i] = 9^i */
    static seq_t *qpow = NULL;
    static size_t qpow_size = 0;

    /* ensure a_arr big enough */
    if (a_arr == NULL) {
        a_size = 1;
        a_arr = (seq_t *)malloc(sizeof(seq_t) * a_size);
        a_arr[0] = 1;                 /* a(0) = 1 */
    } else if ((size_t)n >= a_size) {
        size_t new_sz = a_size;
        while ((size_t)n >= new_sz) new_sz <<= 1;
        a_arr = (seq_t *)realloc(a_arr, sizeof(seq_t) * new_sz);
        for (size_t i = a_size; i < new_sz; ++i) a_arr[i] = 0;
        a_size = new_sz;
    }

    /* ensure qpow big enough */
    if (qpow == NULL) {
        qpow_size = 1;
        qpow = (seq_t *)malloc(sizeof(seq_t) * qpow_size);
        qpow[0] = 1;                 /* 9^0 = 1 */
    } else if ((size_t)n >= qpow_size) {
        size_t new_sz = qpow_size;
        while ((size_t)n >= new_sz) new_sz <<= 1;
        qpow = (seq_t *)realloc(qpow, sizeof(seq_t) * new_sz);
        for (size_t i = qpow_size; i < new_sz; ++i) {
            qpow[i] = qpow[i - 1] * (seq_t)9;
        }
        qpow_size = new_sz;
    }

    /* compute a[k] for all k > a_max up to n */
    for (size_t k = (size_t)(a_max + 1); k <= (size_t)n; ++k) {
        seq_t sum = 1;          /* the chain 0 < (F9)^k itself */
        seq_t g = 1;            /* G(k, 0) = 1 */

        for (size_t d = 1; d < k; ++d) {
            /* Update Gaussian binomial G(k,d) from G(k,d-1) :
               g = g * (qpow[k-d+1]-1) / (qpow[d]-1) */
            seq_t num = qpow[k - d + 1];
            num -= 1;
            seq_t den = qpow[d];
            den -= 1;
            g = (g * num) / den;
            sum += g * a_arr[k - d];
        }
        a_arr[k] = sum;
    }
    a_max = n;    /* last computed n */
    return a_arr[(size_t)n];
}
