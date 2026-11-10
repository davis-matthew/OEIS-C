/*
 * A351250  Numerator of n / A276086(n).
 * https://oeis.org/A351250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = n / gcd(n, A276086(n)); a(0) = 0. A276086(n) has exponent
 *            d_k on the (k+1)-th prime, where d_k are the primorial-base
 *            digits of n
 * Range:     u8 n <= 255, u16 n <= 30030, u32 n <= 30030, u64 n <= 30030,
 *            u128 n <= 30030, big any n
 * Validated: standard, pass, 102 terms through n = 9816 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static const uint64_t pr[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};
    if (n == 0) return (seq_t)0;
    uint64_t N = (uint64_t)n;
    uint64_t w = 1;
    uint64_t g = 1;
    for (int k = 0; k < 16; ++k) {
        uint64_t p = pr[k];
        if (w > N) break;
        uint64_t d = (N / w) % p;
        uint64_t t = N;
        uint64_t e = 0;
        while (e < d && t % p == 0) { t /= p; ++e; }
        for (uint64_t i = 0; i < e; ++i) g *= p;
        if (w > N / p) break;
        w *= p;
    }
    return (seq_t)(N / g);
}
