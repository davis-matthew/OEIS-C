/*
 * A321129  Numerator of Sum_{k=1..n} (k*sin((Pi*k)/3))/sqrt(3).
 * https://oeis.org/A321129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac, easy
 * Formula:   a(n) is computed from the OEIS piecewise rules a(3n + 1) =
 *            (-1)^n, a(6n - 1) = a(6n) = -3n, a(6n + 2) = a(6n + 3) = 6n + 3
 * Range:     i8 n <= 127, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) /* offset 1; index 0 not defined, return 0 */
        return (seq_t)0;

    idx_t mod = n % 6;
    switch (mod) {
        case 1: /* indices 1,4,7,... (3k+1) */
        case 4: {
            idx_t q = (n - 1) / 3;          /* q = n/3 rounded down */
            return (q & 1) ? (seq_t)-1 : (seq_t)1;
        }
        case 5: {                          /* indices 5,11,17,... (6k-1) */
            idx_t k = (n + 1) / 6;
            seq_t kk = (seq_t)k;
            return -3 * kk;
        }
        case 0: {                          /* indices 6,12,18,... (6k) */
            idx_t k = n / 6;
            seq_t kk = (seq_t)k;
            return -3 * kk;
        }
        case 2: {                          /* indices 2,8,14,... (6k+2) */
            idx_t k = (n - 2) / 6;
            seq_t kk = (seq_t)k;
            return 6 * kk + 3;             /* 6k+3 = n+1 */
        }
        case 3: {                          /* indices 3,9,15,... (6k+3) */
            idx_t k = (n - 3) / 6;
            seq_t kk = (seq_t)k;
            return 6 * kk + 3;             /* 6k+3 = n */
        }
        default:
            return (seq_t)0; /* unreachable */
    }
}
