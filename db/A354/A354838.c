/*
 * A354838  For n >= 1, a(n) = A007088(n)/gcd(n, A007088(n)).
 * https://oeis.org/A354838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = binary_to_decimal(n) / gcd(n, binary_to_decimal(n))
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* binary_to_decimal(n) interprets the binary representation of n as a decimal number */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;               /* offset starts at 1; 0 is unused */

    /* Find position of the most significant bit (0‑based). */
    int msb = 0;
    for (idx_t t = n; t > 1; t >>= 1) msb++;

    /* Convert binary string of n to a decimal number. */
    seq_t x = 0;
    for (int i = msb; i >= 0; --i) {
        uint64_t bit = (n >> i) & 1ULL;
        x = x * 10 + (seq_t)bit;
    }

    seq_t g = gcd(x, (seq_t)n); /* gcd from utils can handle seq_t */
    return x / g;
}
