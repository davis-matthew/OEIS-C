/*
 * A249772  Period of the senary (base-6) representation of 1/n, or 0 if 1/n
 *          terminates.
 * https://oeis.org/A249772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = period of the senary (base‑6) representation of 1/n, or 0
 *            if 1/n terminates
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Remove all factors 2 and 3 from the denominator. */
    seq_t m = (seq_t)n;
    while ((m % (seq_t)2) == 0) m /= 2;
    while ((m % (seq_t)3) == 0) m /= 3;
    if (m == 1) return 0;                /* terminates */

    const seq_t base = (seq_t)6;
    seq_t power = (seq_t)1;               /* 6^0 mod m  */
    uint64_t k = 1;

    /* Incrementally compute 6^k mod m to find the smallest k with 6^k ≡ 1 (mod m). */
    while (1) {
        power = (power * base) % m;
        if (power == 1) break;
        ++k;
    }
    return (seq_t)k;
}
