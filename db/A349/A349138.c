/*
 * A349138  Inverse Möbius transform of A349137, where A349137(n) =
 *          phi(A003602(n)).
 * https://oeis.org/A349138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{d|n} A349137(d), where A349137(d) =
 *            phi((oddpart(d)+1)/2)
 * Range:     u8 n <= 404, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 103 terms through n = 9970 (2026-11-10)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient.h"

static seq_t A349137_a(idx_t n)
{
    while ((n & 1) == 0) {
        n >>= 1;
    }
    idx_t k = (n + 1) >> 1;
    return totient(k);
}

seq_t a(idx_t n)
{
    seq_t s = 0;
    for (idx_t i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            s += A349137_a(i);
            idx_t j = n / i;
            if (j != i) {
                s += A349137_a(j);
            }
        }
    }
    return s;
}
