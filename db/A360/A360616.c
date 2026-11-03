/*
 * A360616  Half the number of prime factors of n (counted with multiplicity,
 *          A001222), rounded down.
 * https://oeis.org/A360616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(Ω(n)/2), where Ω(n) is the total number of prime
 *            factors of n (counted with multiplicity – A001222)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-01)
 * Helpers:   bigomega_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bigomega_seq.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    seq_t omega = bigomega_seq((seq_t)n);
    return omega >> 1;   /* integer division by 2, i.e., floor(omega/2) */
}
