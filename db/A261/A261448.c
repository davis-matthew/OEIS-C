/*
 * A261448  Numbers n >= 100 that are divisible by n mod 100.
 * https://oeis.org/A261448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th integer ≥100 that is divisible by its last two
 *            digits (i.e., n % (n % 100) == 0). This implementation searches
 *            upward from 100 until the n‑th qualifying number is found
 * Range:     u8 n <= 18, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t val = 100; ; ++val) {
        idx_t mod = val % 100;
        if (mod && (val % mod) == 0) {
            ++count;
            if (count == n)
                return (seq_t)val;
        }
    }
}
