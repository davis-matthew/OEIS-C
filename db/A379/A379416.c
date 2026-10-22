/*
 * A379416  a(n) = n + [n*r/t] + [n*s/t], where r = 3^(1/4); s = 3^(1/2); t =
 *          3^(3/4) and [ ] = floor.
 * https://oeis.org/A379416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n / 3^(1/4)) + floor(n / sqrt(3))
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* n as the base term */
    seq_t n_seq = (seq_t)n;

    /* ---------------------------------------------------------
       Compute floor( n / sqrt(3) )
       --------------------------------------------------------- */
    unsigned __int128 n2 = (unsigned __int128)n_seq * (unsigned __int128)n_seq;
    seq_t n2_seq = (seq_t)n2;                /* n²   */
    seq_t t2 = n2_seq / 3;                  /* floor(n² / 3)   */
    seq_t v2 = floor_sqrt(t2);              /* floor(sqrt(floor(n² / 3))) */

    /* ---------------------------------------------------------
       Compute floor( n / 3^(1/4) )
       --------------------------------------------------------- */
    unsigned __int128 n4 = n2 * n2;          /* n⁴   */
    seq_t n4_seq = (seq_t)n4;                /* cast to seq_t (fits for n≤10⁴) */
    seq_t t4 = n4_seq / 3;                  /* floor(n⁴ / 3)   */
    seq_t sqrt1 = floor_sqrt(t4);           /* sqrt( floor(n⁴ / 3) )   */
    seq_t v4 = floor_sqrt(sqrt1);           /* floor( sqrt( sqrt( floor(n⁴ / 3) ) ) ) */

    /* ---------------------------------------------------------
       Sum components
       --------------------------------------------------------- */
    return n_seq + v2 + v4;
}
