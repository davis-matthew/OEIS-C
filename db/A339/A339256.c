/*
 * A339256  Leading digit of n in base 6.
 * https://oeis.org/A339256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = floor(n / 6^floor(log_6(n))) (leading digit of n in base
 *            6)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1; n==0 should not occur */
    uint64_t pow = 1;              /* highest power of 6 <= n */
    while (pow <= n / 6ull) {
        pow *= 6ull;
    }
    return (seq_t)((uint64_t)n / pow);   /* leading digit 1‑5 */
}
