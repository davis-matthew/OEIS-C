/*
 * A260109  Expansion of f(x^3) * f(-x^3)^2 * psi(x)^2 / psi(-x) in powers of
 *          x where psi(), f() are Ramanujan theta functions.
 * https://oeis.org/A260109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 125, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   c_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A260109
   Euler transform of the 12‑periodic sequence
   c = [3, -2, 2, -1, 3, -6, 3, -1, 2, -2, 3, -4]
   a(0) = 1,  for n ≥ 1
   a(n) = (1/n) * Σ_{k=1}^{n} d(k) · a(n‑k)
   where d(k) = Σ_{d|k} d · c(d)
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/c_of.h"

/* main function */
seq_t a(idx_t n)
{
    /* static storage for computed terms */
    static seq_t *A = NULL;
    static size_t A_len = 0;     /* number of terms allocated and computed */
    static size_t A_cap = 0;     /* allocated capacity */

    /* static storage for auxiliary d[1..N] */
    static int64_t *d = NULL;
    static size_t d_cap = 0;     /* size of allocated d array */

    /* ensure we have computed up to n */
    if (n >= (idx_t)A_len) {
        size_t N = (size_t)n;          /* use size_t for array indices */

        /* grow A if necessary */
        if (N + 1 > A_cap) {
            size_t new_cap = N + 1;
            if (new_cap < 256) new_cap = 256;
            A = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
            A_cap = new_cap;
        }

        /* recompute d[1..N] if needed */
        if (N + 1 > d_cap) {
            d = (int64_t *)realloc(d, (N + 1) * sizeof(int64_t));
            d_cap = N + 1;
            for (size_t i = 0; i <= N; ++i) d[i] = 0;
            for (size_t k = 1; k <= N; ++k) {
                int c = c_of((int)k);
                int64_t val = (int64_t)k * (int64_t)c;
                for (size_t m = k; m <= N; m += k) {
                    d[m] += val;
                }
            }
        }

        /* initialise a[0] if not yet */
        if (A_len == 0) {
            A[0] = (seq_t)1;
            A_len = 1;
        }

        /* compute a[k] for k = A_len .. N */
        for (size_t k = A_len; k <= N; ++k) {
            long long sum = 0;
            for (size_t j = 1; j <= k; ++j) {
                sum += d[j] * (long long)A[k - j];
            }
            /* division is exact because of the definition */
            A[k] = (seq_t)(sum / (long long)k);
        }
        A_len = N + 1;
    }

    return A[(size_t)n];
}
