/*
 * A347481  Number of total dominating sets in the n-dipyramidal graph.
 * https://oeis.org/A347481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(n) + 2*cos(n*Pi/2) + 3*2^n - 3 (Lucas(0)=2,
 *            Lucas(1)=1, Lucas(n)=Lucas(n-1)+Lucas(n-2))
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"   /* big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    /* Compute Lucas(n) */
    seq_t lucas;
    if (n == 1) {
        lucas = 1;                 /* Lucas(1) */
    } else {
        seq_t L0 = 2;               /* Lucas(0) */
        seq_t L1 = 1;               /* Lucas(1) */
        seq_t Li;
        for (idx_t i = 2; i <= n; ++i) {
            Li  = L1 + L0;          /* L(i) = L(i-1) + L(i-2) */
            L0   = L1;
            L1   = Li;
        }
        lucas = L1;
    }

    /* Compute 3*2^n */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);
    seq_t term3 = pow2;
    term3 += pow2;
    term3 += pow2;                  /* term3 = 3 * 2^n */

    /* Sum the components */
    seq_t sum = lucas + term3;

    /* Add 2*cos(n*Pi/2) which is 0, 2, or -2 */
    /* cos(n*Pi/2) pattern: 0, -1, 0, 1 for n = 1,2,3,4,... */
    /* So 2*cos is 0, -2, 0, 2 ... */
    switch (n & 3) {                /* n mod 4 */
        case 0:  /* n % 4 == 0 -> cos = 1 -> +2 */
            sum += 2;
            break;
        case 2:  /* n % 4 == 2 -> cos = -1 -> -2 */
            sum -= 2;
            break;
        default: /* n % 4 == 1 or 3 -> cos = 0 -> +0 */
            break;
    }

    /* Subtract 3 */
    sum -= 3;

    return sum;
}
