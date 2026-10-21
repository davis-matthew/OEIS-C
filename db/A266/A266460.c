/*
 * A266460  Binary representation of the n-th iteration of the "Rule 27"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: For even n:
 *     a(n) = 10^(n/2)
 * For odd n:
 *     a(n) = (10 * (100^n - 9 * 10^floor(n/2)) - 1) / 9
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {
        /* even n */
        int exp = (int)(n >> 1);          /* n/2 */
        return pow_seq((seq_t)10, exp);   /* 10^(n/2) */
    } else {
        /* odd n */
        int exp100 = (int)n;              /* n */
        seq_t pow100 = pow_seq((seq_t)100, exp100);     /* 100^n */
        int exp10 = (int)(n >> 1);        /* floor(n/2) */
        seq_t pow10 = pow_seq((seq_t)10, exp10);        /* 10^floor(n/2) */
        seq_t term = pow100 - pow10 * (seq_t)9;         /* 100^n - 9*10^floor(n/2) */
        term = term * (seq_t)10;                /* 10 * ( ... ) */
        term = term - (seq_t)1;                /* - 1 */
        return term / (seq_t)9;                /* divide by 9 */
    }
}
