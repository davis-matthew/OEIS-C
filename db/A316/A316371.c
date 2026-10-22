/*
 * A316371  G.f.: A(x) = Sum_{n>=0} binomial(3*(n+1), n)/(n+1) * x^n /
 *          (1+x)^(2*(n+1)).
 * https://oeis.org/A316371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 32, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A316371
 *
 *  Recurrence (derived from (1 - x + x^2) A(x) = 1 + 3*x^2*A(x)^2 + x^3*A(x)^3 ),
 *  a(0) = 1, a(1) = 1,
 *  for n ≥ 2
 *      a(n) = a(n-1) - a(n-2) + 3*conv2[n-2] + (n≥3 ? triple[n-3] : 0)
 *  where
 *      conv2[k] = Σ_{i=0}^{k} a(i)*a(k-i)   (unordered convolution)
 *      triple[k] = Σ_{i=0}^{k} a(i)*conv2(k-i)
 *  The function keeps a memoised table of a(n) and conv2.  All operations are
 *  performed in the type seq_t (which may be 8‑, 16‑, 32‑, 64‑bit or
 *  arbitrary‑precision).  No memory or time is wasted beyond the
 *  O(n^2) arithmetic required to build all values up to n.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *A       = NULL;   /* a[0 .. a_size-1]        */
    static seq_t *conv2   = NULL;   /* conv2[0 .. conv_size-1] */
    static size_t a_size  = 0;      /* current number of a terms stored */
    static size_t conv_size = 0;    /* current length allocated for conv2 */

    size_t need = (size_t)n + 1;    /* how many terms are required */

    if (need > a_size) {
        /* allocate/expand arrays */
        size_t old_a = a_size;
        size_t old_c = conv_size;

        size_t new_a = need;
        size_t new_c = 2 * need;     /* conv2 indices go up to 2*need-2 */

        seq_t *tmpA = (seq_t *)realloc(A, new_a * sizeof(seq_t));
        if (!tmpA) abort();
        memset(tmpA + old_a, 0, (new_a - old_a) * sizeof(seq_t));
        A = tmpA;   a_size = new_a;

        seq_t *tmpC = (seq_t *)realloc(conv2, new_c * sizeof(seq_t));
        if (!tmpC) abort();
        memset(tmpC + old_c, 0, (new_c - old_c) * sizeof(seq_t));
        conv2 = tmpC;   conv_size = new_c;

        /* compute new terms */
        for (size_t idx = old_a; idx < need; ++idx) {
            if (idx == 0) {          /* a(0) = 1 */
                A[0] = 1;
                conv2[0] = A[0] * A[0];
                continue;
            }
            if (idx == 1) {          /* a(1) = 1 */
                A[1] = 1;
            } else {                 /* idx >= 2 */
                seq_t double_sum = conv2[idx - 2];

                seq_t triple_sum = 0;
                if (idx >= 3) {
                    for (size_t i = 0; i <= idx - 3; ++i) {
                        triple_sum += A[i] * conv2[(idx - 3) - i];
                    }
                }

                seq_t rhs = double_sum * 3 + triple_sum;
                seq_t temp = A[idx - 1] + rhs;
                A[idx] = temp - A[idx - 2];
            }

            /* update conv2 with new contributions from A[idx] */
            for (size_t j = 0; j <= idx; ++j) {
                if (j == idx) {
                    conv2[2 * idx] += A[idx] * A[idx];
                } else {
                    conv2[idx + j] += 2 * A[idx] * A[j];
                }
            }
        }
    }

    return A[n];
}
