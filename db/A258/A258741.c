/*
 * A258741  Expansion of f(x^3, x^5) / f(x, x^3) in powers of x where f(, ) is
 *          Ramanujan's general theta function.
 * https://oeis.org/A258741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in ∏_{m≥1} (1−x^m)^e(m) where e(m) = 1
 *            if m ≡ 1 or 7 (mod 8); −1 if m ≡ 4 (mod 8) or m ≡ 2 or 14 (mod
 *            16); 0 otherwise
 * Range:     i8 n <= 32, i16 n <= 94, i32 n <= 300, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Allocate array [0..n] for coefficients */
    size_t N = (size_t)n;          /* cast to size_t for malloc */
    seq_t *coeff = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!coeff) return (seq_t)0;   /* allocation failure – unlikely for the ranges considered */
    coeff[0] = (seq_t)1;           /* initial term a(0) = 1 */

    /* Compute contributions from all m <= n */
    for (size_t m = 1; m <= N; ++m) {
        int sign = 0;
        size_t mod8 = m & 7;        /* m % 8 */
        if (mod8 == 1 || mod8 == 7) {
            sign = 1;
        } else {
            size_t mod16 = m & 15; /* m % 16 */
            if (mod8 == 4 || mod16 == 2 || mod16 == 14) {
                sign = -1;
            }
        }
        if (sign == 0) continue;

        if (sign == 1) {
            /* Multiply by (1 - x^m): decrement coefficients from high to low */
            for (size_t i = N; i >= m; --i) {
                coeff[i] -= coeff[i - m];
            }
        } else { /* sign == -1, multiply by 1/(1 - x^m): add coefficients from low to high */
            for (size_t i = m; i <= N; ++i) {
                coeff[i] += coeff[i - m];
            }
        }
    }

    seq_t result = coeff[N];
    free(coeff);
    return result;
}
