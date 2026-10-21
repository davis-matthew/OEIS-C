/*
 * A249118  Position of 32n^6 in the ordered union of {h^6, h >=1} and
 *          {32*k^6, k >=1}.
 * https://oeis.org/A249118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( (32)^(1/6) * n ) + n The position of 32*n^6 in the
 *            ordered union of {h^6 : h≥1} and {32*k^6 : k≥1}. This uses only
 *            integer arithmetic
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   _floor_sixth_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_floor_sixth_root.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* value below the offset is irrelevant */
    idx_t hmax = _floor_sixth_root(n);   /* floor((32)^(1/6) * n) */
    seq_t n_t  = (seq_t)n;
    return (seq_t)hmax + n_t;      /* position of 32*n^6 in the union */
}
