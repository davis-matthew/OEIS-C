/*
 * A359467  a(n) = (A166486(n)+A353627(n)) mod 2.
 * https://oeis.org/A359467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A166486(n) + A353627(n)) mod 2, where A166486(n) = [n
 *            mod 4 != 0] and A353627(n) = [odd part of n is squarefree]
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 141 terms through n = 9132 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t m = n;
    unsigned s = (n & 3) ? 1u : 0u;

    /* strip factors of 2 */
    while (m && (m & 1) == 0)
        m >>= 1;

    /* test squarefreeness of the odd part */
    unsigned sf = 1;
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            m /= p;
            if (m % p == 0) {
                sf = 0;
                break;
            }
        }
    }
    return (seq_t)((s + sf) & 1u);
}
