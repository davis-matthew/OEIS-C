/*
 * A379742  a(n) is the number of divisors of A000178(n).
 * https://oeis.org/A379742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{primes p <= n} (1 + e_p), where e_p =
 *            Sum_{k=1..n} v_p(k!) is the exponent of p in A000178(n); a(n) =
 *            A000005(A000178(n))
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 74,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 948 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    seq_t result = 1;
    unsigned char *comp;
    uint64_t p, k;

    if (n < 2)
        return result;

    comp = (unsigned char *)calloc((size_t)n + 1, 1);
    if (!comp)
        return result;

    for (p = 2; p <= (uint64_t)n; p++) {
        uint64_t e = 0;
        if (comp[p])
            continue;
        for (k = p * p; k <= (uint64_t)n; k += p)
            comp[k] = 1;
        /* each multiple k of p adds v_p(k) to the exponent of every factorial j! with j >= k */
        for (k = p; k <= (uint64_t)n; k += p) {
            uint64_t m = k, v = 0;
            while (m % p == 0) {
                m /= p;
                v++;
            }
            e += v * ((uint64_t)n - k + 1);
        }
        result *= (seq_t)(e + 1);
    }
    free(comp);
    return result;
}
