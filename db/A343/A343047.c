/*
 * A343047  a(n) = A343046(n, n).
 * https://oeis.org/A343047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n (*) n, the lunar (max/min) square of n in primorial
 *            base: digits of n (primorial base) d_i; result digit r_k = max
 *            over i+j=k of min(d_i,d_j); a(n) = sum r_k * prod_{m<k}
 *            prime(m+1). a(0)=0
 * Range:     u8 n <= 11, u16 n <= 84, u32 n <= 2309, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 9491 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static const unsigned int A343047_PRIMES[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97, 101, 103, 107, 109, 113,
    127, 131, 137, 139, 149, 151, 157, 163, 167, 173
};

seq_t a(idx_t n)
{
    uint8_t d[64];
    uint8_t res[128] = {0};
    size_t len = 0;
    uint64_t v = (uint64_t)n;
    for (size_t p = 0; p < 40 && v > 0; ++p) {
        d[len++] = (uint8_t)(v % A343047_PRIMES[p]);
        v /= A343047_PRIMES[p];
    }
    for (size_t i = 0; i < len; ++i) {
        for (size_t j = 0; j < len; ++j) {
            uint8_t m = d[i] < d[j] ? d[i] : d[j];
            if (m > res[i + j]) res[i + j] = m;
        }
    }
    size_t reslen = 2 * len;
    seq_t value = 0;
    seq_t weight = 1;
    for (size_t k = 0; k < reslen; ++k) {
        if (res[k]) value += (seq_t)res[k] * weight;
        if (k + 1 < reslen) weight *= (seq_t)A343047_PRIMES[k];
    }
    return value;
}
