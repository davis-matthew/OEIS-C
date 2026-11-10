/*
 * A320000  Square array A(n, k) read by descending antidiagonals: A(1, 1) =
 *          2, A(1, k) = 1 for k > 1, and for n > 1, A(n, k) = Sum_{d|n, d>=k}
 *          A010051(1+d)*[Sum_{i=0..valuation(n,1+d)} A((n/d)/((1+d)^i),
 *          1+d)].
 * https://oeis.org/A320000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(1,1)=2, A(1,k)=1 for k>1; A(n,k) = Sum_{d|n, d>=k}
 *            A010051(1+d)*Sum_{i=0..valuation(n,1+d)} A((n/d)/(1+d)^i, 1+d);
 *            read by descending antidiagonals A(1,s),A(2,s-1),...,A(s,1)
 * Range:     u8 n <= 10585, u16 n <= 10585, u32 n <= 10585, u64 n <= 10585,
 *            u128 n <= 10585, big any n
 * Validated: standard, pass, 121 terms through n = 9747 (2026-11-08)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t A320000_arr(uint64_t n, uint64_t k) {
    if (n == 1) return (seq_t)(k == 1 ? 2 : 1);
    seq_t sum = 0;
    for (uint64_t d = k; d <= n; d++) {
        if (n % d) continue;
        uint64_t p = d + 1;
        if (!is_prime((idx_t)p)) continue;
        uint64_t m = n / d;
        sum += A320000_arr(m, p);
        while (m % p == 0) {
            m /= p;
            sum += A320000_arr(m, p);
        }
    }
    return sum;
}

seq_t a(idx_t n) {
    uint64_t s = 1;
    while ((uint64_t)s * (s + 1) / 2 < (uint64_t)n) s++;
    uint64_t j = (uint64_t)n - s * (s - 1) / 2;
    return A320000_arr(j, s + 1 - j);
}
