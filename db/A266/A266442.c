/*
 * A266442  Binary representation of the n-th iteration of the "Rule 25"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11101*10^(n/2) for even n>2 a(n) = (10*100^n -
 *            909999*10^((n+1)/2) - 1)/9 for odd n>3 Special cases:
 *            a(0)=a(1)=1, a(2)=1010, a(3)=1000011
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0 || n == 1) return 1;
    if (n == 2) return 1010;
    if (n == 3) return 1000011;

    if ((n & 1ULL) == 0) {           /* even n > 2 */
        seq_t pow = pow10(n / 2);
        return 11101 * pow;
    } else {                         /* odd n >= 5 */
        idx_t double_n = 2 * n;
        seq_t pow2n = pow10(double_n);          /* 10^(2n) */
        seq_t term1 = 10 * pow2n;                /* 10^(2n+1) */
        idx_t half = (n + 1) / 2;
        seq_t powHalf = pow10(half);             /* 10^((n+1)/2) */
        seq_t term2 = 909999 * powHalf;
        seq_t numer = term1 - term2 - 1;
        return numer / 9;
    }
}
