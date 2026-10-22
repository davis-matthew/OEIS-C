/*
 * A321775  Number of compositions of n into parts with distinct
 *          multiplicities and with exactly five parts.
 * https://oeis.org/A321775
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into exactly five parts each
 *            part is a positive integer, where all multiplicities of distinct
 *            part values are distinct. The terms for n ≥ 5 are given by a(n)
 *            = [5|n] // one composition when n is a multiple of 5 +
 *            5·(⌊(n−1)/4⌋ − δ) // patterns of the form 4+1 + 1...
 * Range:     u8 n <= 88, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 5) return 0;         /* offset starts at 5, below are undefined */

    seq_t result = 0;

    /* pattern [5] */
    if ((n % 5) == 0) result += (seq_t)1;

    /* pattern [4,1] */
    idx_t k   = (n - 1) / 4;          /* floor((n-1)/4) */
    idx_t sub = ((n % 5) == 0) ? 1 : 0;
    if (k > sub) {
        result += (seq_t)(5 * (k - sub));
    }

    /* pattern [3,2] */
    idx_t max_a   = (n >= 2) ? (n - 2) / 3 : 0;   /* floor((n-2)/3) */
    idx_t first   = (n & 1) ? 1 : 2;              /* first a with same parity as n */
    idx_t count_p = 0;
    if (first <= max_a) {
        count_p = ((max_a - first) / 2) + 1;      /* number of a with correct parity */
    }

    idx_t count_a = count_p;
    if ((n % 5) == 0) {
        idx_t a_equal = n / 5;
        if (a_equal <= max_a) {
            count_a--;                           /* exclude a = b case */
        }
    }

    result += (seq_t)(10 * count_a);

    return result;
}
