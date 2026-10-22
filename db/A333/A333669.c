/*
 * A333669  The smallest nontrivial quadratic residue modulo n.
 * https://oeis.org/A333669
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 if 2 is a quadratic residue mod n; otherwise: 3 if 3 is
 *            a quadratic residue mod n; otherwise 4
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_qr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_qr.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 5, but we return 0 for smaller n */
    if (n < 5) return 0;

    if (is_qr(n, 2)) return (seq_t)2;
    if (is_qr(n, 3)) return (seq_t)3;
    return (seq_t)4;
}
