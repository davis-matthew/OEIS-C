/*
 * A342288  a(n) = C(n)*C(n+2), where C(n) is the n-th Catalan number
 *          A000108(n).
 * https://oeis.org/A342288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n)*C(n+2), where C(k) is the k‑th Catalan number
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    seq_t cn  = catalan(n);
    seq_t cn2 = catalan(n + 2);
    return cn * cn2;
}
