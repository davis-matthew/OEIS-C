/*
 * A244845  Binary representation of 4^n - 2^(n+1) - 1.
 * https://oeis.org/A244845
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the decimal number obtained from the binary
 *            representation of 4^n – 2^(n+1) – 1. In binary the number
 *            consists of all 1's except for a single 0 at bit position n+1
 *            (counting from the least–significant bit). This function builds
 *            the decimal result by scanning the 2n bits from most–signific...
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                 /* undefined below the offset */

    const idx_t total_bits = 2 * n;   /* number of binary bits */
    seq_t result = 0;
    bool seen_nonzero = false;

    for (idx_t pos = total_bits; pos > 0; --pos) {
        idx_t bit_index = pos - 1;          /* 0‑based from LSB */
        int bit = (bit_index == n + 1) ? 0 : 1;

        if (!seen_nonzero) {
            if (bit == 0)           /* skip the single leading zero for n==2 */
                continue;
            seen_nonzero = true;
        }
        result = result * 10 + bit;
    }

    return result;
}
