/*
 * A334871  Number of steps needed to reach 1 when starting from n and
 *          iterating with A334870.
 * https://oeis.org/A334871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; for n > 1 write n = m^2 * r with r squarefree (r =
 *            A007913(n)); then a(n) = A048675(r) + (m > 1 ? 1 + a(m) : 0),
 *            which follows from a(n) = A048675(A007913(n)) + a(A008833(n))
 *            and a(k^2) = 1 + a(k)
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 136, u64 n <= 312,
 *            u128 n <= 726, big any n
 * Validated: standard, pass, 92 terms through n = 9593 (2026-11-08)
 * Helpers:   pow2, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    uint64_t x = (uint64_t)n;
    while (x > 1) {
        uint64_t m = 1;
        uint64_t p;
        for (p = 2; p <= x / p; p++) {
            if (x % p == 0) {
                unsigned e = 0;
                while (x % p == 0) {
                    x /= p;
                    e++;
                }
                for (unsigned i = 0; i < e / 2; i++) m *= p;
                if (e & 1) {
                    idx_t pi = (idx_t)prime_pi((idx_t)p);
                    total += pow2(pi - 1);
                }
            }
        }
        if (x > 1) {
            /* remaining prime factor with exponent 1 */
            idx_t pi = (idx_t)prime_pi((idx_t)x);
            total += pow2(pi - 1);
        }
        if (m > 1) total += 1;
        x = m;
    }
    return total;
}
