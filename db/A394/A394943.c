/*
 * A394943  Number of well-formed bracketed words of total length n built from
 *          the symbol * (of length 1) and three unary bracket types (), [],
 *          and {}, with no empty bracket pair, where every maximal chain of
 *          unary brackets is written in canonical weakly increasing order.
 * https://oeis.org/A394943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is defined by the recurrence from the OEIS entry
 *            a(0)=a(1)=a(2)=1; for n≥3: a(n)=f(n-1)+3f(n-2)-3f(n-4)+f(n-6)
 *            +3∑_{k=1}^{n-3}a(k)a(n-2-k) -3∑_{k=1}^{n-5}a(k)a(n-4-k)
 *            +∑_{k=1}^{n-7}a(k)a(n-6-k), where f(m)=a(m) for m≥1 and f(m)=0
 *            for m≤0. The sums can be expressed via the convolution
 *            S(m)=∑_...
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   mul_small_fast
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mul_small_fast.h"

/* Computes the n‑th term of OEIS A394943. */
seq_t a(idx_t n)
{
    /* Handle the trivial case. */
    if (n == 0)
        return (seq_t)1;

    /* Allocate array for a[0..n] and convolution conv[0..n]. */
    seq_t *a_arr = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *conv  = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!a_arr || !conv) {
        free(a_arr);
        free(conv);
        return (seq_t)0;      /* Allocation failure – return 0. */
    }

    /* Base value a(0)=1 and its contribution to conv. */
    a_arr[0] = (seq_t)1;
    conv[0]  = a_arr[0] * a_arr[0];   /* a0*a0 */

    /* Compute successive terms. */
    for (idx_t i = 1; i <= n; ++i) {
        /* Base cases for i=1,2. */
        if (i == 1 || i == 2) {
            a_arr[i] = (seq_t)1;
        } else {
            /* Compute a[i] using the recurrence. */
            seq_t val = (seq_t)0;

            /* f(i-1) */
            val += a_arr[i - 1];

            /* 3*f(i-2) */
            val += mul_small_fast(a_arr[i - 2], 3);

            /* -3*f(i-4)  (f(i-4)=a[i-4] when i≥5) */
            if (i >= 5)
                val -= mul_small_fast(a_arr[i - 4], 3);

            /* +f(i-6)  (f(i-6)=a[i-6] when i≥7) */
            if (i >= 7)
                val += a_arr[i - 6];

            /* +3*(S(i-2)-2a(i-2)) */
            if (i >= 2) {
                seq_t tmp = conv[i - 2];
                tmp -= mul_small_fast(a_arr[i - 2], 2);
                val += mul_small_fast(tmp, 3);
            }

            /* -3*(S(i-4)-2a(i-4)) */
            if (i >= 5) {
                seq_t tmp = conv[i - 4];
                tmp -= mul_small_fast(a_arr[i - 4], 2);
                val -= mul_small_fast(tmp, 3);
            }

            /* +(S(i-6)-2a(i-6)) */
            if (i >= 7) {
                seq_t tmp = conv[i - 6];
                tmp -= mul_small_fast(a_arr[i - 6], 2);
                val += tmp;
            }

            a_arr[i] = val;
        }

        /* Update the convolution array with the new term a[i]. */
        for (idx_t k = 0; k < i; ++k) {
            idx_t idx = i + k;
            if (idx > n)
                break;   /* Out of range – no effect on future sums. */
            seq_t prod = a_arr[i] * a_arr[k];
            prod = mul_small_fast(prod, 2);  /* double count for i≠k */
            conv[idx] += prod;
        }

        /* Add the a[i]*a[i] term to conv[2i] if it falls within range. */
        idx_t idx2 = i << 1;   /* 2*i */
        if (idx2 <= n)
            conv[idx2] += a_arr[i] * a_arr[i];
    }

    seq_t result = a_arr[n];
    free(a_arr);
    free(conv);
    return result;
}
