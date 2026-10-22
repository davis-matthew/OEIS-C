/*
 * A347856  a(n) = (6*n)!/((4*n)!*n!) * (n/2)!/(3*n/2)!.
 * https://oeis.org/A347856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n)! / ((4*n)! * n!) * (n/2)! / (3*n/2)!
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 5, u64 n <= 11, u128 n <= 13,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (used for computation):                               */
/*   a(0) = 1,  a(1) = 20                                            */
/*   a(2*k)   = 54*(6*k-1)*(6*k-5)*(12*k-1)*(12*k-5)*(12*k-7)*(12*k-11)  */
/*              / [k*(2*k-1)*(8*k-1)*(8*k-3)*(8*k-5)*(8*k-7)] * a(2*k-2)   */
/*   a(2*k+1) = 216*(9*k^2-1)*(144*k^2-1)*(144*k^2-25)             */
/*              / [k*(2*k+1)*(64*k^2-1)*(64*k^2-9)] * a(2*k-1)          */

#include "../utils/seq_gcd.h"

static seq_t gcd_local(seq_t a, seq_t b) { return seq_gcd(a, b); }

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 20;

    seq_t prev2 = 1;      /* a(i-2) */
    seq_t prev1 = 20;     /* a(i-1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        if ((i & 1ULL) == 0) {           /* even: i = 2*k */
            idx_t k = i >> 1;
            seq_t num = 54;
            num *= (seq_t)(6 * k - 1);
            num *= (seq_t)(6 * k - 5);
            num *= (seq_t)(12 * k - 1);
            num *= (seq_t)(12 * k - 5);
            num *= (seq_t)(12 * k - 7);
            num *= (seq_t)(12 * k - 11);

            seq_t den = (seq_t)k;
            den *= (seq_t)(2 * k - 1);
            den *= (seq_t)(8 * k - 1);
            den *= (seq_t)(8 * k - 3);
            den *= (seq_t)(8 * k - 5);
            den *= (seq_t)(8 * k - 7);

            seq_t g = gcd_local(num, den);
            num /= g;
            den /= g;

            seq_t tmp = prev2 / den;
            cur = tmp * num;
        } else {                          /* odd: i = 2*k+1 */
            idx_t k = (i - 1) >> 1;
            seq_t ksq = (seq_t)k * k;

            seq_t num = 216;
            num *= (seq_t)(9 * ksq - 1);
            num *= (seq_t)(144 * ksq - 1);
            num *= (seq_t)(144 * ksq - 25);

            seq_t den = (seq_t)k;
            den *= (seq_t)(2 * k + 1);
            den *= (seq_t)(64 * ksq - 1);
            den *= (seq_t)(64 * ksq - 9);

            seq_t g = gcd_local(num, den);
            num /= g;
            den /= g;

            seq_t tmp = prev2 / den;
            cur = tmp * num;
        }

        /* shift values for next iteration */
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
