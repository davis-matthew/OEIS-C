/*
 * A344846  Sum of the prime numbers appearing along the border of an n X n
 *          square array whose elements are the numbers from 1..n^2, listed in
 *          increasing order by rows.
 * https://oeis.org/A344846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the prime numbers appearing along the border of
 *            the n×n square array whose elements are the numbers 1..n² filled
 *            row‑wise
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)           /* not part of the OEIS offset, but safe */
        return 0;

    if (n == 1)           /* only element is 1, which is not prime */
        return 0;

    seq_t sum = 0;

    /* ---------- top row ---------- */
    for (idx_t val = 1; val <= n; ++val)
        if (is_prime(val))
            sum += (seq_t)val;

    /* ---------- middle rows: left & right columns ---------- */
    for (idx_t r = 2; r <= n - 1; ++r) {
        idx_t left  = (r - 1) * n + 1;   /* first element of row r */
        idx_t right = r * n;             /* last  element of row r */

        if (is_prime(left))
            sum += (seq_t)left;
        if (is_prime(right))
            sum += (seq_t)right;
    }

    /* ---------- bottom row ---------- */
    for (idx_t val = (n - 1) * n + 1; val <= n * n; ++val)
        if (is_prime(val))
            sum += (seq_t)val;

    return sum;
}
