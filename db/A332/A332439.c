/*
 * A332439  Primitive period of the partial sums of the periodic unsigned
 *          Schick sequence for N = 7 (A130794), taken modulo 14, and the
 *          related Euler tour using all regular 14-gon vertices.
 * https://oeis.org/A332439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, walk
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A332439 – primitive period of partial sums of the periodic
   unsigned Schick sequence for N = 7, modulo 14.  The Schick sequence
   is periodic with period 3:  S = [1, 5, 3].  The partial sums
   a(n) are defined by
        a(0) = 0
        a(n) = ( ∑_{j=0}^{n-1} S[j] ) mod 14   for n ≥ 1.
   The sequence thus has period 42 because 3·14 and 9 (the sum of one
   period of S) are coprime with 14.  This implementation uses the
   closed‑form calculation below. */

#include <stdint.h>

static const seq_t _prefill[3] = { 0, 1, 6 };   /* sums for 0,1,2 terms */

seq_t a(idx_t n)
{
    /* compute number of full periods (length 3) and remaining terms */
    idx_t full_cycles = n / 3;
    idx_t remainder   = n % 3;

    /* sum of one full period of S is 1 + 5 + 3 = 9   */
    /* we reduce full_cycles modulo 14 to avoid large intermediate values */
    seq_t sum = ( (seq_t)(full_cycles % 14) * 9 + _prefill[remainder] ) % 14;
    return sum;
}
