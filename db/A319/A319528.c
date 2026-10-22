/*
 * A319528  a(n) = 8 * sigma(n).
 * https://oeis.org/A319528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 * sigma(n)
 * Range:     u8 n <= 17, u16 n <= 2519, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* guard for out‑of‑range indices */
    return sigma_of(n) * (seq_t)8;
}
