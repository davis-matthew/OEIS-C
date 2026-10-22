/*
 * A329200  The ghost iteration (A): add or subtract the number formed by
 *          absolute differences of digits (A040115), according to parity
 *          (even or odd).
 * https://oeis.org/A329200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n + (-1)^d * d, where d = A040115(n) – number formed by
 *            absolute differences of consecutive decimal digits of n. For
 *            single‑digit n, d = 0
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   _a040115
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/_a040115.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t d = _a040115(n);

    /* If d is even, add; if odd, subtract. */
    if ((d & (seq_t)1) == 0)
        return N + d;
    else
        return N - d;
}
