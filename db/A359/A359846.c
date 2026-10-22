/*
 * A359846  a(n) = (((5 - (n mod 2))*10^(3 + n*(9/2) - (n mod 2)*(5/2)))^2 +
 *          2)/81.
 * https://oeis.org/A359846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (((5 - (n mod 2))*10^(3 + n*(9/2) - (n mod 2)*(5/2)))^2 +
 *            2)/81 This simplifies to: a(2k) = (25*10^(6+18k) + 2)/81 a(2k+1)
 *            = (16*10^(10+18k) + 2)/81 The function computes the value using
 *            arbitrary‑precision arithmetic provided by the build
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 1, u128 n <= 3,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"   /* seq_t pow_int(seq_t base, idx_t exp) */

seq_t a(idx_t n)
{
    /* n even: use 25*10^(6+18k)+2 / 81 where k = n/2 */
    if ((n & 1ULL) == 0) {
        idx_t k   = n >> 1;                     /* k = n/2                     */
        idx_t exp = 6 + 18 * k;                  /* exponent for 10^exp           */
        seq_t pow10 = pow_int((seq_t)10, exp);   /* 10^exp                       */
        seq_t val = pow10;
        val *= (seq_t)25;                        /* 25 * 10^exp                  */
        val += (seq_t)2;                         /* 25 * 10^exp + 2              */
        val /= (seq_t)81;                        /* divide by 81 (exact)         */
        return val;
    }

    /* n odd: use 16*10^(10+18k)+2 / 81 where k = (n-1)/2 */
    {
        idx_t k   = n >> 1;                     /* floor((n-1)/2)              */
        idx_t exp = 10 + 18 * k;                /* exponent for 10^exp           */
        seq_t pow10 = pow_int((seq_t)10, exp);  /* 10^exp                       */
        seq_t val = pow10;
        val *= (seq_t)16;                        /* 16 * 10^exp                  */
        val += (seq_t)2;                         /* 16 * 10^exp + 2              */
        val /= (seq_t)81;                        /* divide by 81 (exact)         */
        return val;
    }
}
