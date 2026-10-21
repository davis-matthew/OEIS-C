/*
 * A289421  Number of Dyck paths of semilength n and height exactly 8.
 * https://oeis.org/A289421
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   number of Dyck paths of semilength n and height exactly 8.
 *            Computed via dynamic programming: the number of paths of length
 *            2n that stay in [0,…,k] is obtained by two passes up and down.
 *            The desired value is f(8) – f(7), where f(k) is the number of
 *            length‑2n Dyck paths of height ≤ k. The algorithm...
 * Range:     u8 n <= 10, u16 n <= 13, u32 n <= 21, u64 n <= 37, u128 n <= 71,
 *            big any n
 * Helpers:   _compute_f
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_compute_f.h"

seq_t a(idx_t n)
{
    if (n < 8) return (seq_t)0;        /* offset is 8 */

    seq_t f8 = _compute_f(n, 8);
    seq_t f7 = _compute_f(n, 7);
    return f8 - f7;                    /* paths of height exactly 8 */
}
