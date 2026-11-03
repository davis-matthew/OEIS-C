/*
 * A254526  Fourier expansion of first basis element of space of weight 1
 *          modular forms on Gamma1(12).
 * https://oeis.org/A254526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 12 * sum_{d | n} chi(d) for n > 0 a(0) = 1 where chi(d) =
 *            1 if d ≡ 5 (mod 12) -1 if d ≡ 7 (mod 12) 0 otherwise
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9333 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Character χ modulo 12 */
static inline int chi(idx_t d)
{
    switch ((unsigned int)(d % 12)) {
    case 5: return 1;
    case 7: return -1;
    default: return 0;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)               /* constant term of the q‑expansion */
        return 1;

    seq_t sum = 0;

    /* iterate over divisors up to sqrt(n) without overflow */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            int c = chi(i);
            if (c != 0) sum += (seq_t)c;

            idx_t j = n / i;
            if (j != i) {
                c = chi(j);
                if (c != 0) sum += (seq_t)c;
            }
        }
    }
    return (seq_t)12 * sum;
}
