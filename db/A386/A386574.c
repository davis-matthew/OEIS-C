/*
 * A386574  Number of squarefree balanced divisors of n.
 * https://oeis.org/A386574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} c(d)*mu(d)^2, c(d)=1 if phi(d) | sigma(d) else
 *            0 (A351114); enumerate squarefree divisors of n
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-10)
 * Helpers:   factorize
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factorize.h"

seq_t a(idx_t n)
{
    uint64_t pr[32];
    int ex[32];
    size_t k = factorize((uint64_t)n, pr, ex);
    seq_t count = 0;
    uint64_t total = (uint64_t)1 << k;
    for (uint64_t mask = 0; mask < total; ++mask) {
        seq_t phi = 1, sig = 1;
        for (size_t i = 0; i < k; ++i) {
            if (mask & ((uint64_t)1 << i)) {
                phi = phi * (seq_t)(pr[i] - 1);
                sig = sig * (seq_t)(pr[i] + 1);
            }
        }
        if (sig % phi == 0)
            count++;
    }
    return count;
}
