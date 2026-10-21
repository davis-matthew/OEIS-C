/*
 * A286654  Characteristic sequence of the Beatty sequence, A022841, of
 *          sqrt(7).
 * https://oeis.org/A286654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((n+1)/sqrt(7)) - ceil(n/sqrt(7)) Equivalent to: a(n)
 *            = 1 if floor((n+1)/sqrt(7)) > floor(n/sqrt(7)), else 0
 *            Computation uses integer arithmetic only: floor(n/sqrt(7)) =
 *            floor_sqrt( (n*n)/7 )
 * Range:     u8 n <= 20, u16 n <= 313, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Offset 1: a(1) is the first term.  For n==0 just return 0. */
    if (n == 0)
        return 0;

    /* Convert to seq_t once for the arithmetic */
    seq_t tn  = (seq_t)n;
    seq_t tn1 = tn + 1;

    /* Use 128‑bit intermediate for the product to avoid overflow (when
       seq_t is a 64‑bit unsigned type).  The division is performed in
       128‑bit arithmetic, then the result is converted back to seq_t. */
    seq_t sq0 = (seq_t)(((unsigned __int128)tn * tn) / 7);
    seq_t sq1 = (seq_t)(((unsigned __int128)tn1 * tn1) / 7);

    /* Floor sqrt of the reduced integer gives floor(n / sqrt(7)) etc.   */
    seq_t k0 = floor_sqrt(sq0);   /* floor(n / sqrt(7)) */
    seq_t k1 = floor_sqrt(sq1);   /* floor((n+1) / sqrt(7)) */

    /* The characteristic is 1 iff the floor value increases.  */
    return (k1 > k0) ? (seq_t)1 : (seq_t)0;
}
