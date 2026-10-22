/*
 * A385366  a(n) = Sum_{permutations p of [n]} des(p^2), where des(p) is the
 *          number of descents of p.
 * https://oeis.org/A385366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n ≤ 2, otherwise a(n) = (n‑1)! * (n² – n – 4) / 2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n <= 2) return 0;          /* offset cases */

    seq_t fn   = factorial(n - 1);  /* (n-1)!        */
    seq_t nn   = (seq_t)n;          /* cast n into seq_t */
    seq_t factor = nn * nn - nn - 4;/* n² – n – 4    */
    seq_t res   = fn * factor;      /* product       */
    res /= 2;                       /* division by 2 */
    return res;
}
