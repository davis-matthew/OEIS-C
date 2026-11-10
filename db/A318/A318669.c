/*
 * A318669  Numerators of the sequence whose Dirichlet convolution with itself
 *          yields A065769 ("Prime cascade").
 * https://oeis.org/A318669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, mult
 * Formula:   a(n) = numerator of f(n), f multiplicative, f(1)=1, f(n) =
 *            (A065769(n) - Sum_{d|n, 1<d<n} f(d)f(n/d))/2; computed per prime
 *            power with H(k)=2^(2k-1)f(p^k)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 8191, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 103 terms through n = 8888 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static uint64_t A318669_prev_prime(uint64_t p)
{
    if (p <= 2) return 1;
    for (uint64_t x = p - 1; x > 1; --x) {
        bool pr = true;
        for (uint64_t d = 2; d * d <= x; ++d)
            if (x % d == 0) { pr = false; break; }
        if (pr) return x;
    }
    return 1;
}

static seq_t A318669_prime_power(uint64_t p, uint64_t e)
{
    uint64_t q = A318669_prev_prime(p);
    seq_t *H = mem_new(seq_t, e + 1);
    for (uint64_t k = 1; k <= e; ++k) {
        seq_t g = (seq_t)q;
        for (uint64_t j = 1; j < k; ++j) g *= (seq_t)p;
        for (uint64_t j = 0; j < 2 * k - 2; ++j) g *= 2;
        seq_t s = 0;
        for (uint64_t i = 1; i < k; ++i) s += H[i] * H[k - i];
        H[k] = g - s;
    }
    seq_t h = H[e];
    mem_free(H);
    uint64_t v = 2 * e - 1;
    while (v > 0 && h % 2 == 0) { h /= 2; --v; }
    return h;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 1;
    uint64_t m = (uint64_t)n;
    seq_t result = 1;
    for (uint64_t d = 2; d * d <= m; ++d) {
        if (m % d == 0) {
            uint64_t e = 0;
            while (m % d == 0) { m /= d; ++e; }
            result *= A318669_prime_power(d, e);
        }
    }
    if (m > 1) result *= A318669_prime_power(m, 1);
    return result;
}
