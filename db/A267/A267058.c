/*
 * A267058  Decimal representation of the n-th iteration of the "Rule 97"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 256^⌊n/4⌋ * (((n%2==0)?((n/2+1)%2):(((n-1)/2+1)%2))) +
 *            20*256^⌊n/4⌋ * (((n%2==0)?((n/2)%2):(((n-1)/2)%2))) -
 *            ((n%2)?1:0)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    idx_t q = n / 4;    /* floor(n/4)   */
    idx_t m = n / 2;    /* floor(n/2)   */

    /* 256^q */
    seq_t pow256 = pow_seq((seq_t)256, (int)q);
    /* 20*256^q   */
    seq_t pow20  = pow256 * (seq_t)20;

    /* first component: 256^q * ((m+1) mod 2)  */
    seq_t part1 = pow256 * ((seq_t)((m + 1) & 1));
    /* second component: 20*256^q * (m mod 2)   */
    seq_t part2 = pow20  * ((seq_t)(m & 1));

    seq_t result = part1 + part2;

    /* subtract 1 for odd n */
    if (n & 1)
        result -= (seq_t)1;

    return result;
}
