/*
 * A267080  Coefficient of x^2 in the minimal polynomial of the continued
 *          fraction [1^n,2^(1/3),1,1,...], where 1^n means n ones.
 * https://oeis.org/A267080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 14*a(n-1) + 90*a(n-2) - 350*a(n-3) + 90*a(n-4) * +
 *            14*a(n-5) - a(n-6), n > 7 * Initial values: * a(0) = -6, a(1) =
 *            -66, a(2) = 1110, a(3) = 18318, * a(4) = 333750, a(5) = 5938446,
 *            a(6) = 106865274, * a(7) = 1915425570
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 7, i64 n <= 14, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return a(n) for A267080 */
seq_t a(idx_t n)
{
    /* Static cache to store computed values */
    static seq_t *A = NULL;          /* array of values */
    static idx_t  A_len = 0;         /* number of values stored (max index + 1) */
    static idx_t  A_cap = 0;         /* allocated capacity */

    /* Initialise cache with first 8 terms if needed */
    if (A == NULL) {
        A_cap = 8;
        A_len = 8;
        A = (seq_t *)malloc((size_t)A_cap * sizeof(seq_t));
        if (!A) return 0; /* allocation failure, return 0 as a fallback */
        A[0] = (seq_t)-6;
        A[1] = (seq_t)-66;
        A[2] = (seq_t)1110;
        A[3] = (seq_t)18318;
        A[4] = (seq_t)333750;
        A[5] = (seq_t)5938446;
        A[6] = (seq_t)106865274;
        A[7] = (seq_t)1915425570;
    }

    /* Ensure the cache is large enough */
    if (n >= A_cap) {
        idx_t new_cap = A_cap;
        while (n >= new_cap) new_cap <<= 1; /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(A, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        A = tmp;
        A_cap = new_cap;
    }

    /* If value already computed, return it */
    if (n < A_len) {
        return A[n];
    }

    /* Compute missing terms up to n */
    for (idx_t i = A_len; i <= n; ++i) {
        /* Apply recurrence for i >= 8 */
        if (i >= 8) {
            A[i] = (seq_t)14 * A[i-1]
                 + (seq_t)90 * A[i-2]
                 - (seq_t)350 * A[i-3]
                 + (seq_t)90 * A[i-4]
                 + (seq_t)14 * A[i-5]
                 - A[i-6];
        } else {
            /* Should not happen because initial 8 are already set */
            A[i] = 0;
        }
    }
    A_len = n + 1;
    return A[n];
}
