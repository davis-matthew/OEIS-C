/*
 * A323160  a(n) = gcd(n, A323159(n)) = gcd(n, A034448(n), A048250(n)).
 * https://oeis.org/A323160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, A034448(n), A048250(n)), where A034448(n) =
 *            prod_{p^e||n} (p^e+1) and A048250(n) = prod_{p|n} (p+1)
 * Range:     u8 n <= 3070, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9308 (2026-11-08)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    idx_t m = n;
    seq_t U = 1, S = 1;

    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            seq_t pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)p;
            }
            U *= (pw + 1);
            S *= ((seq_t)p + 1);
        }
    }
    if (m > 1) {
        U *= ((seq_t)m + 1);
        S *= ((seq_t)m + 1);
    }

    return gcd((seq_t)n, gcd(U, S));
}
