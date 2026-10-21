/*
 * A277250  Largest number that is n times sum of its digits when written in
 *          base 4.
 * https://oeis.org/A277250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 3876, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: largest integer x with x = n * digit_sum_base4(x)   */
/* (sum of base-4 digits).  If none exists, 0 is returned.   */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    const int S_UPPER = 1024;          /* sufficient upper bound for all n in practice */
    seq_t nseq   = (seq_t) n;
    seq_t best   = 0;                   /* default 0 if no satisfying number exists */

    for (int S = S_UPPER; S >= 1; --S) {
        seq_t x   = nseq * (seq_t) S;   /* candidate value */
        seq_t sum = 0;
        seq_t tmp = x;

        /* digit sum in base 4 */
        while (tmp) {
            sum += tmp % 4;
            tmp /= 4;
        }

        if (sum == (seq_t) S) {         /* found a valid x */
            best = x;
            break;                       /* larger S → larger x, so stop */
        }
    }
    return best;
}
