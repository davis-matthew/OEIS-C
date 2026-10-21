/*
 * A251589  a(n) = 9^(n-7) * (n+1)^(n-9) * (262144*n^7 + 2494464*n^6 +
 *          10470208*n^5 + 25229505*n^4 + 37857568*n^3 + 35537670*n^2 +
 *          19414368*n + 4782969).
 * https://oeis.org/A251589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9^(n-7) * (n+1)^(n-9) * (262144*n^7 + 2494464*n^6 +
 *            10470208*n^5 + 25229505*n^4 + 37857568*n^3 + 35537670*n^2 +
 *            19414368*n + 4782969)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (All operations are exact, handling negative exponents by dividing the final result.) */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Convert the index to the sequence integer type */
    seq_t n_seq = (seq_t)n;

    /* Compute powers of n up to n^7 */
    seq_t n2 = n_seq * n_seq;
    seq_t n3 = n2 * n_seq;
    seq_t n4 = n3 * n_seq;
    seq_t n5 = n4 * n_seq;
    seq_t n6 = n5 * n_seq;
    seq_t n7 = n6 * n_seq;

    /* Polynomial part P(n) */
    seq_t P =
        (seq_t)262144 * n7
      + (seq_t)2494464 * n6
      + (seq_t)10470208 * n5
      + (seq_t)25229505 * n4
      + (seq_t)37857568 * n3
      + (seq_t)35537670 * n2
      + (seq_t)19414368 * n_seq
      + (seq_t)4782969;

    /* Exponents for 9 and (n+1) */
    int64_t e1 = (int64_t)n - 7;   /* exponent of 9 */
    int64_t e2 = (int64_t)n - 9;   /* exponent of (n+1) */

    /* Positive and negative parts of the powers */
    seq_t pow9_pos  = (seq_t)1;
    seq_t powN1_pos = (seq_t)1;
    seq_t pow9_neg  = (seq_t)1;
    seq_t powN1_neg = (seq_t)1;

    if (e1 >= 0)
        pow9_pos = big_pow((seq_t)9, (seq_t)e1);
    else
        pow9_neg = big_pow((seq_t)9, (seq_t)(-e1));

    if (e2 >= 0)
        powN1_pos = big_pow(n_seq + (seq_t)1, (seq_t)e2);
    else
        powN1_neg = big_pow(n_seq + (seq_t)1, (seq_t)(-e2));

    /* Build numerator and denominator */
    seq_t numerator   = P * pow9_pos * powN1_pos;
    seq_t denominator = pow9_neg * powN1_neg;

    /* Final integer result (division is exact) */
    return numerator / denominator;
}
