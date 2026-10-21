/*
 * A286496  Renyi-Ulam liar numbers: maximum k such that n questions "Is x in
 *          subset S of {1,...,k}?" are guaranteed to determine x when at most
 *          one answer can be a lie.
 * https://oeis.org/A286496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): maximum k such that n questions with at most one lie can identify an element in {1..k}.  
   Computed as the maximum of the largest even and odd integers below the bounds
   f_even(n)=2^n/(n+1) and f_odd(n)=(2^n-n+1)/(n+1).  All calculations are
   performed in seq_t as required; no floating point is used.
*/

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute 2^n in seq_t form */
    seq_t pow2q = pow2(n);
    /* common denominator: n+1 */
    seq_t denom = (seq_t)(n + 1);

    /* floor(2^n/(n+1)) */
    seq_t even_floor = pow2q / denom;
    /* adjust to the largest even <= even_floor */
    seq_t cand_even = (even_floor & (seq_t)1) ? (even_floor - (seq_t)1) : even_floor;

    /* floor((2^n - n + 1)/(n+1)) */
    seq_t odd_num = pow2q - (seq_t)n + (seq_t)1;
    seq_t odd_floor = odd_num / denom;
    /* adjust to the largest odd <= odd_floor */
    seq_t cand_odd = (odd_floor & (seq_t)1) ? odd_floor : (odd_floor - (seq_t)1);

    /* return the maximum candidate */
    return (cand_even > cand_odd) ? cand_even : cand_odd;
}
