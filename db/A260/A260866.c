/*
 * A260866  Base-16 representation of a(n) is the concatenation of the base-16
 *          representations of 1, 2, ..., n, n-1, ..., 1.
 * https://oeis.org/A260866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the integer whose base‑16 representation is the *
 *            concatenation of the base‑16 representations of 1,2,…,n,n‑1,…,1.
 *            * (a(0)=0). * * Implementation detail: build the number digit by
 *            digit in base‑16. * For each integer x (1 ≤ x ≤ n) we repeatedly
 *            take its most * significant hex digit, multip...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   append_hex_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/append_hex_digits.h"

seq_t a(idx_t n)
{
    seq_t acc = 0;
    if (n == 0)
        return acc;

    /* Forward part: 1 .. n */
    for (idx_t i = 1; i <= n; ++i)
        acc = append_hex_digits(acc, i);

    /* Backward part: n‑1 .. 1 */
    if (n > 1)
        for (idx_t i = n - 1; i >= 1; --i)
            acc = append_hex_digits(acc, i);

    return acc;
}
