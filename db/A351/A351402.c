/*
 * A351402  G.f. A(x) satisfies: 1 / (1 - x) = Product_{i>=1, j>=1}
 *          A(x^(i*j)).
 * https://oeis.org/A351402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1/n) * Σ_{k=1..n} b(k) * a(n-k) with b(1) = 1 and b(k) =
 *            1 - Σ_{m|k, m<k} τ(k/m) * (k/m) * b(m), where τ(t) is the number
 *            of divisors of t
 * Range:     i8 n <= 12, i16 n <= 48, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Persistent static arrays to enable memoisation */
    static seq_t *A = NULL;           /* array of a values   */
    static seq_t *B = NULL;           /* array of b values   */
    static idx_t *TAU = NULL;         /* divisor count array */
    static idx_t capacity = 0;        /* current allocated size   */
    static idx_t last_a = 0;          /* greatest index of a computed */
    static idx_t last_b = 0;          /* greatest index of b computed */

    /* initial allocation for n == 0 (a(0) = 1) */
    if (A == NULL) {
        capacity = 1;
        A = (seq_t *)malloc(capacity * sizeof(seq_t));
        B = (seq_t *)malloc(capacity * sizeof(seq_t));
        TAU = (idx_t *)malloc(capacity * sizeof(idx_t));
        A[0] = (seq_t)1;   /* a(0) = 1 */
        last_a = 0;
        last_b = 0;
    }

    /* Expand storage if required */
    if ((idx_t)n >= capacity) {
        idx_t new_cap = (idx_t)n + 1;

        /* Reallocate arrays */
        A  = (seq_t *)realloc(A,  new_cap * sizeof(seq_t));
        B  = (seq_t *)realloc(B,  new_cap * sizeof(seq_t));
        TAU = (idx_t *)realloc(TAU, new_cap * sizeof(idx_t));
        capacity = new_cap;

        /* Reset divisor counts */
        for (idx_t i = 0; i < capacity; ++i) TAU[i] = 0;
        for (idx_t i = 1; i < capacity; ++i)
            for (idx_t j = i; j < capacity; j += i)
                ++TAU[j];

        /* Compute b-values for new indices */
        for (idx_t k = last_b + 1; k < capacity; ++k) {
            seq_t sum = 0;
            /* enumerate proper divisors m of k */
            for (idx_t d = 1; d * d <= k; ++d) {
                if (k % d == 0) {
                    idx_t m1 = d;
                    if (m1 < k) {
                        idx_t t = k / m1;
                        sum += (seq_t)B[m1] * ((seq_t)TAU[t] * (seq_t)t);
                    }
                    idx_t m2 = k / d;
                    if (m2 != d && m2 < k) {
                        idx_t t = k / m2;
                        sum += (seq_t)B[m2] * ((seq_t)TAU[t] * (seq_t)t);
                    }
                }
            }
            B[k] = (seq_t)1 - sum;
        }
        last_b = n;

        /* Compute a-values for new indices */
        for (idx_t i = last_a + 1; i < capacity; ++i) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k)
                sum += B[k] * A[i - k];
            A[i] = sum / (seq_t)i;
        }
        last_a = n;
    }

    return A[n];
}
