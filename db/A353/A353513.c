/*
 * A353513  a(n) = 1 if A328572(n) is of the form 4m+3, and 0 otherwise.
 * https://oeis.org/A353513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if A328572(n) == 3 (mod 4), else 0; A328572(n) =
 *            Product_{i} prime_i^(d_i-1) over nonzero base-primorial digits
 *            d_i of n. Computed mod 4 directly
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 137 terms through n = 9804 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A353513_is_prime(uint64_t x)
{
    uint64_t d;
    if (x < 2) return 0;
    for (d = 2; d * d <= x; d++)
        if (x % d == 0) return 0;
    return 1;
}

seq_t a(idx_t n)
{
    uint64_t rem = (uint64_t)n;
    uint64_t p = 2;
    int r = 1; /* product mod 4 */
    while (rem > 0) {
        uint64_t d = rem % p;
        uint64_t e = d > 0 ? d - 1 : 0;
        if ((p & 3) == 3 && (e & 1)) r = (r * 3) & 3;
        rem /= p;
        p++;
        while (!A353513_is_prime(p)) p++;
    }
    return (seq_t)(r == 3 ? 1 : 0);
}
