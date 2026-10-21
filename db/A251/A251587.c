/*
 * A251587  a(n) = 7^(n-5) * (n+1)^(n-7) * (1296*n^5 + 9720*n^4 + 30555*n^3 +
 *          50665*n^2 + 44621*n + 16807).
 * https://oeis.org/A251587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^(n-5) * (n+1)^(n-7) * (1296*n^5 + 9720*n^4 + 30555*n^3
 *            + 50665*n^2 + 44621*n + 16807)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 7, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* cast the index to seq_t for arithmetic */
    seq_t n_seq = (seq_t)n;
    seq_t n1 = n_seq + (seq_t)1;

    /* powers of n */
    seq_t n2 = n_seq * n_seq;                /* n^2   */
    seq_t n3 = n2 * n_seq;                  /* n^3   */
    seq_t n4 = n3 * n_seq;                  /* n^4   */
    seq_t n5 = n4 * n_seq;                  /* n^5   */

    /* polynomial part */
    seq_t poly = (seq_t)1296 * n5
               + (seq_t)9720 * n4
               + (seq_t)30555 * n3
               + (seq_t)50665 * n2
               + (seq_t)44621 * n_seq
               + (seq_t)16807;

    /* exponents for the 7 and (n+1) factors */
    idx_t exp7_num = (n >= 5) ? n - 5 : (idx_t)0;
    idx_t exp7_den = (n < 5)  ? 5 - n : (idx_t)0;
    idx_t exp1_num = (n >= 7) ? n - 7 : (idx_t)0;
    idx_t exp1_den = (n < 7)  ? 7 - n : (idx_t)0;

    /* compute powers (use big_pow for safety with large exponents) */
    seq_t pow7_num = exp7_num ? big_pow((seq_t)7, exp7_num) : (seq_t)1;
    seq_t pow7_den = exp7_den ? big_pow((seq_t)7, exp7_den) : (seq_t)1;
    seq_t pow1_num = exp1_num ? big_pow(n1, exp1_num) : (seq_t)1;
    seq_t pow1_den = exp1_den ? big_pow(n1, exp1_den) : (seq_t)1;

    /* numerator: poly * 7^(n-5) * (n+1)^(n-7) */
    seq_t num = poly * pow7_num;
    num = num * pow1_num;

    /* denominator: 7^(5-n) * (n+1)^(7-n) */
    seq_t den = pow7_den * pow1_den;

    /* exact division */
    return num / den;
}
