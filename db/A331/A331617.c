/*
 * A331617  E.g.f.: exp(1 / (1 - arctan(x)) - 1).
 * https://oeis.org/A331617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sum_{k=1..n} binomial(n-1,k-1) * A191700(k) * a(n-k);
 *            a(0)=1
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache for computed values. */
    static seq_t *A = NULL;          /* a[0..] */
    static seq_t *C = NULL;          /* A191700 values, C[0]=1 (dummy) */
    static idx_t cap = 0;            /* allocated size - 1 */
    static idx_t max_computed = 0;   /* highest index already computed */
    static bool initialized = false;

    /* Initial allocation (index 0). */
    if (!initialized) {
        cap = 1;
        A = (seq_t *)realloc(NULL, sizeof(seq_t) * (cap + 1));
        C = (seq_t *)realloc(NULL, sizeof(seq_t) * (cap + 1));
        if (!A || !C) return 0;          /* allocation failure */
        A[0] = 1;
        C[0] = 1;                        /* dummy value for c[0] */
        initialized = true;
    }

    /* Grow cache if necessary. */
    if (n > cap) {
        idx_t new_cap = cap;
        while (new_cap <= n) new_cap <<= 1;
        A = (seq_t *)realloc(A, sizeof(seq_t) * (new_cap + 1));
        C = (seq_t *)realloc(C, sizeof(seq_t) * (new_cap + 1));
        if (!A || !C) return 0;          /* allocation failure */
        cap = new_cap;
    }

    /* Compute missing entries up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* Compute C[i] = A191700(i) */
        seq_t ck = 0;
        seq_t prod = 0;          /* product for the current odd i */
        for (idx_t odd = 1; odd <= i; odd += 2) {
            if (odd == 1) {
                prod = (seq_t)i;
            } else {
                prod = prod * (seq_t)(i - (odd - 2));
                prod = prod * (seq_t)(i - (odd - 1));
            }
            seq_t term = C[i - odd] * (prod / (seq_t)odd);
            if (((odd - 1) / 2) & 1)
                term = -term;
            ck += term;
        }
        C[i] = ck;

        /* Compute A[i] using the recurrence. */
        seq_t sum = 0;
        seq_t binom = 1;   /* binomial(i-1,0) */
        for (idx_t k = 1; k <= i; ++k) {
            sum += binom * C[k] * A[i - k];
            if (k < i)
                binom = (binom * (seq_t)(i - k)) / (seq_t)k; /* binomial(i-1,k) */
        }
        A[i] = sum;
    }
    if (n > max_computed) max_computed = n;

    return A[n];
}
