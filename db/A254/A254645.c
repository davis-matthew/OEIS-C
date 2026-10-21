/*
 * A254645  Fourth partial sums of sixth powers (A001014).
 * https://oeis.org/A254645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)^2*(n+3)*(n+4)*(-1 - 8*n + 14*n^2 + 8*n^3 +
 *            n^4)/5040
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 19, u64 n <= 196, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The value is computed by multiplying all factors and cancelling the prime
 * divisors of 5040 (2^4 * 3^2 * 5 * 7) during the multiplication to avoid
 * intermediate overflow on 64‑bit builds. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* cast index to seq_t first */
    seq_t nn = (seq_t)n;

    /* compute n^2, n^3, n^4 */
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;

    /* compute the polynomial: n^4 + 8*n^3 + 14*n^2 - (8*n + 1) */
    seq_t poly = n4;
    poly += (seq_t)8 * n3;
    poly += (seq_t)14 * n2;
    poly -= ((seq_t)8 * nn + (seq_t)1);

    /* list of multiplicative factors for the numerator */
    seq_t factors[7];
    factors[0] = nn;
    factors[1] = nn + (seq_t)1;
    factors[2] = nn + (seq_t)2;
    factors[3] = nn + (seq_t)2;     /* (n+2)^2 */
    factors[4] = nn + (seq_t)3;
    factors[5] = nn + (seq_t)4;
    factors[6] = poly;

    /* prime factorisation of the denominator 5040 = 2^4 * 3^2 * 5 * 7 */
    unsigned rem2  = 4;
    unsigned rem3  = 2;
    unsigned rem5  = 1;
    unsigned rem7  = 1;

    seq_t result = (seq_t)1;

    for (int i = 0; i < 7; ++i) {
        seq_t f = factors[i];

        /* cancel factor 2 */
        while (rem2 && (f % (seq_t)2) == (seq_t)0) {
            f /= (seq_t)2;
            --rem2;
        }

        /* cancel factor 3 */
        while (rem3 && (f % (seq_t)3) == (seq_t)0) {
            f /= (seq_t)3;
            --rem3;
        }

        /* cancel factor 5 */
        while (rem5 && (f % (seq_t)5) == (seq_t)0) {
            f /= (seq_t)5;
            --rem5;
        }

        /* cancel factor 7 */
        while (rem7 && (f % (seq_t)7) == (seq_t)0) {
            f /= (seq_t)7;
            --rem7;
        }

        result *= f;
    }

    /* If any denominator factors are still left (unlikely), divide now */
    while (rem2) { result /= (seq_t)2; --rem2; }
    while (rem3) { result /= (seq_t)3; --rem3; }
    while (rem5) { result /= (seq_t)5; --rem5; }
    while (rem7) { result /= (seq_t)7; --rem7; }

    return result;
}
