/*
 * A373155  a(n) = 1 if n is a non-multiple of 3 whose 2-adic valuation is
 *          even, otherwise 0.
 * https://oeis.org/A373155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = 1 if n is a non‑multiple of 3 whose 2‑adic valuation is
 *            even, otherwise 0
 * Range:     u8 n <= 9087, u16 n <= 9087, u32 n <= 9087, u64 n <= 9087,
 *            u128 n <= 9087, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t types are provided by the build environment. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                         /* safe fallback for n < 1 */

    if (n % 3 == 0)                       /* multiple of 3 → 0 */
        return 0;

    /* compute the 2‑adic valuation (number of trailing zeros) */
    idx_t v = 0;
    idx_t m = n;
    while ((m & 1ULL) == 0) {             /* while m is even */
        v++;
        m >>= 1;
    }

    return (v % 2 == 0) ? (seq_t)1 : (seq_t)0;
}
