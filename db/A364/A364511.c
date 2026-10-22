/*
 * A364511  a(n) = (6*n)!*(5*n)!*(2*n)! / ((4*n)!^2 * (3*n)! * n!^2).
 * https://oeis.org/A364511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n)!*(5*n)!*(2*n)! / ((4*n)!^2*(3*n)!*n!^2) Recurrence
 *            (from the OEIS entry): a(0) = 1 a(n) = a(n-1) *
 *            (15*(5*n-1)*(5*n-2)*(5*n-3)*(5*n-4)*(6*n-1)*(6*n-5)) /
 *            (4*(4*n-1)^2*(4*n-3)^2*n^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute GCD for arbitrary seq_t types. */
static inline seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;

    seq_t result = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        /* numerator factors for this step */
        seq_t numerators[7];
        numerators[0] = (seq_t)(5*k - 1);
        numerators[1] = (seq_t)(5*k - 2);
        numerators[2] = (seq_t)(5*k - 3);
        numerators[3] = (seq_t)(5*k - 4);
        numerators[4] = (seq_t)(6*k - 1);
        numerators[5] = (seq_t)(6*k - 5);
        numerators[6] = (seq_t)15;

        /* denominator factors for this step */
        seq_t denominators[7];
        denominators[0] = (seq_t)(4*k - 1);
        denominators[1] = (seq_t)(4*k - 1);
        denominators[2] = (seq_t)(4*k - 3);
        denominators[3] = (seq_t)(4*k - 3);
        denominators[4] = (seq_t)k;
        denominators[5] = (seq_t)k;
        denominators[6] = (seq_t)4;

        /* multiply by numerators and cancel with denominators immediately */
        for (int i = 0; i < 7; ++i) {
            result *= numerators[i];
            for (int j = 0; j < 7; ++j) {
                if (denominators[j] > (seq_t)1) {
                    seq_t g = seq_gcd(result, denominators[j]);
                    if (g > (seq_t)1) {
                        result /= g;
                        denominators[j] /= g;
                    }
                }
            }
        }

        /* any remaining denominator factors must divide the result exactly */
        for (int j = 0; j < 7; ++j) {
            if (denominators[j] > (seq_t)1) {
                result /= denominators[j];
            }
        }
    }
    return result;
}
