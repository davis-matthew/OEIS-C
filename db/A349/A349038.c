/*
 * A349038  G.f. A(x) satisfies: A(x) = 1 / (1 + x - 2 * x * A(-2*x)).
 * https://oeis.org/A349038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1 a(n) = -a(n-1) - Sum_{k=0..n-1} (-2)^(k+1) * a(k) *
 *            a(n-k-1) for n≥1 This implements the recurrence from the
 *            generating‑function definition A(x) = 1/(1 + x - 2x A(-2x))
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 7, i64 n <= 10, i128 n <= 15,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t* A      = NULL;         /* Array of a(n) values   */
    static size_t  cap    = 0;           /* Allocated capacity     */
    static size_t  used   = 0;           /* Number of terms stored */

    size_t need = (size_t)n + 1;         /* Capacity required       */

    /* Resize cache if not big enough */
    if (cap < need) {
        size_t newcap = (cap == 0) ? need : (cap * 2 < need ? need : cap * 2);
        seq_t* newA   = alloc_seq_array(newcap);

        if (A) {                          /* Copy old data           */
            memcpy(newA, A, used * sizeof(seq_t));
            free_seq_array(A);
        }

        A   = newA;
        cap = newcap;
    }

    /* Initialise first term */
    if (used == 0) {
        A[0] = 1;
        used = 1;
    }

    /* Compute missing terms up to n */
    for (idx_t i = used; i <= n; ++i) {
        seq_t sum = 0;
        seq_t power = -2;                 /* (-2)^(k+1) for k=0  */

        /* Convolution sum */
        for (idx_t k = 0; k < i; ++k) {
            seq_t prod = A[k];
            prod *= A[i - k - 1];
            prod *= power;
            sum += prod;
            power *= -2;                 /* Next power of -2    */
        }

        A[i] = -A[i - 1] - sum;          /* Recurrence formula  */
    }

    used = (size_t)n + 1;
    return A[n];
}
