/*
 * A347088  a(n) = A055155(n) - d(n), where A055155(n) = Sum_{d|n} gcd(d, n/d)
 *          and d(n) gives the number of divisors of n.
 * https://oeis.org/A347088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A055155(n) - A000005(n) (sum of gcd(d, n/d) over all
 *            divisors of n minus the number of divisors)
 * Range:     u8 n <= 3599, u16 n <= 9513, u32 n <= 9513, u64 n <= 9513,
 *            u128 n <= 9513, big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd64.h"

/* 64‑bit Euclidean algorithm (works for any seq_t values up to 2^64−1). */

seq_t a(idx_t n) {
    seq_t sum = 0;
    seq_t dcnt = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;
            uint64_t g = gcd64(d, e);
            sum += (seq_t)g;
            if (d != e) {
                sum += (seq_t)g;
                dcnt += 2;
            } else {
                dcnt += 1;
            }
        }
    }
    sum -= dcnt;
    return sum;
}
