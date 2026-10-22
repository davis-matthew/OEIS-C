/*
 * A363180  Number of permutations of [2n] with n parity changes.
 * https://oeis.org/A363180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies (2*n - 3)*a(n) = 4*(2*n^2 - 4*n + 1)*a(n-1) +
 *            16*(n-2)^2*(n-1)^2*(2*n - 1)*a(n-2), with a(0)=1, a(1)=2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t */

seq_t a(idx_t n)
{
    /* handle the two base cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* last two computed values of the sequence */
    seq_t a_nm2 = (seq_t)1; /* a(0) */
    seq_t a_nm1 = (seq_t)2; /* a(1) */
    seq_t a_n   = 0;        /* placeholder for a(i) during the loop */

    for (idx_t i = 2; i <= n; ++i) {
        /* cast i to seq_t to use in arithmetic */
        seq_t ii = (seq_t)i;
        seq_t two = (seq_t)2;

        /* denominator: (2*i - 3) */
        seq_t D = two * ii - (seq_t)3;

        /* compute 4*(2*i^2 - 4*i + 1) */
        seq_t i2 = ii * ii; /* i^2, fits in 64-bit for all i <= 10000 */
        seq_t coeff1 = (seq_t)4 * (two * i2 - (seq_t)4 * ii + (seq_t)1);

        /* compute 16*(i-2)^2*(i-1)^2*(2*i - 1) */
        seq_t t1 = (seq_t)(i - 2);
        t1 = t1 * t1;            /* (i-2)^2 */
        seq_t t2 = (seq_t)(i - 1);
        t2 = t2 * t2;            /* (i-1)^2 */
        seq_t t3 = two * ii - (seq_t)1; /* (2*i - 1) */
        seq_t coeff2 = (seq_t)16 * t1 * t2 * t3;

        /* numerator: coeff1*a_nm1 + coeff2*a_nm2 */
        seq_t term1 = coeff1 * a_nm1;
        seq_t term2 = coeff2 * a_nm2;
        seq_t numer = term1 + term2;

        /* compute a(i) = numer / D (exact division) */
        a_n = numer / D;

        /* shift the sliding window */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
