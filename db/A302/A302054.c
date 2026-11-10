/*
 * A302054  a(n) is the sum of prime divisors of A302033(n).
 * https://oeis.org/A302054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A008472(A302033(n)) = Sum_{k : bit k of A003188(n) is
 *            set} prime(k+1), where A003188(n) = n XOR floor(n/2) (binary
 *            Gray code)
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 104 terms through n = 7057 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    uint64_t g = (uint64_t)(n ^ (n >> 1));
    seq_t sum = 0;
    uint64_t p = 2;
    while (g) {
        if (g & 1ULL) sum += (seq_t)p;
        g >>= 1;
        if (!g) break;
        /* advance p to the next prime */
        for (;;) {
            ++p;
            bool ok = true;
            for (uint64_t d = 2; d * d <= p; ++d) {
                if (p % d == 0) { ok = false; break; }
            }
            if (ok) break;
        }
    }
    return sum;
}
