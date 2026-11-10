/*
 * A309526  a(n) is the greatest divisor of A001353(n) that is coprime to
 *          A001353(m) for all positive integers m < n.
 * https://oeis.org/A309526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest divisor of F(n) coprime to F(d) for all proper
 *            divisors d of n, where F = A001353, F(n) = 4*F(n-1) - F(n-2),
 *            F(0) = 0, F(1) = 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 31 (2026-11-08)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t *f = mem_new(seq_t, (size_t)n + 2);
    f[0] = 0;
    f[1] = 1;
    for (idx_t i = 2; i <= n; i++) f[i] = 4 * f[i - 1] - f[i - 2];
    seq_t x = f[n];
    for (idx_t d = 2; d < n; d++) {
        if (n % d != 0) continue;
        seq_t g = gcd(x, f[d]);
        while (g > 1) {
            x /= g;
            g = gcd(x, g);
        }
    }
    mem_free(f);
    return x;
}
