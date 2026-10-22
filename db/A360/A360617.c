/*
 * A360617  Half the number of prime factors of n (counted with multiplicity,
 *          A001222), rounded up.
 * https://oeis.org/A360617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(Ω(n)/2)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    seq_t omega = bigomega(n);
    return (omega + 1) / 2;
}
