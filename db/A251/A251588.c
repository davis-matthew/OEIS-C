/*
 * A251588  a(n) = 8^(n-6) * (n+1)^(n-8) * (16807*n^6 + 143031*n^5 +
 *          525875*n^4 + 1074745*n^3 + 1294846*n^2 + 876856*n + 262144).
 * https://oeis.org/A251588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8^(n-6) * (n+1)^(n-8) * (16807*n^6 + 143031*n^5 +
 *            525875*n^4 + 1074745*n^3 + 1294846*n^2 + 876856*n + 262144)
 * Range:     u8 none, u16 none, u32 n <= 6, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* evaluate the degree‑6 polynomial by Horner's rule */
    seq_t nn = (seq_t)n;
    seq_t P = 16807ULL;
    P = P * nn + 143031ULL;
    P = P * nn + 525875ULL;
    P = P * nn + 1074745ULL;
    P = P * nn + 1294846ULL;
    P = P * nn + 876856ULL;
    P = P * nn + 262144ULL;

    /* 8^(n-6)   (numerator / denominator separately) */
    seq_t pow8_num = 1;
    seq_t pow8_den = 1;
    if (n >= 6)
        pow8_num = pow_seq(8ULL, (int)(n - 6));
    else
        pow8_den = pow_seq(8ULL, (int)(6 - n));

    /* (n+1)^(n-8)  (numerator / denominator separately) */
    seq_t pown1_num = 1;
    seq_t pown1_den = 1;
    if (n >= 8)
        pown1_num = pow_seq((seq_t)(n + 1), (int)(n - 8));
    else
        pown1_den = pow_seq((seq_t)(n + 1), (int)(8 - n));

    /* multiply all numerators and divide by all denominators */
    seq_t numerator = pow8_num * pown1_num;
    numerator = numerator * P;
    seq_t denominator = pow8_den * pown1_den;

    return numerator / denominator;
}
