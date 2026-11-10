/*
 * A341016  Numbers k such that A124440(k) is a multiple of A066840(k).
 * https://oeis.org/A341016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 2 such that A124440(k) == 0 (mod A066840(k));
 *            found by a linear search, using A124440(k) = k*phi(k)/2 -
 *            A066840(k) and inclusion-exclusion for A066840
 * Range:     u8 n <= 14, u16 n <= 264, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 81 terms through n = 4409 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static seq_t A341016_half_sum(uint64_t m, seq_t *phi_out)
{
    uint64_t half = m / 2;
    uint64_t primes[16];
    size_t cnt = 0;
    uint64_t tmp = m;
    seq_t phi = (seq_t)m;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            primes[cnt++] = p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) primes[cnt++] = tmp;
    for (size_t i = 0; i < cnt; ++i)
        phi = phi / (seq_t)primes[i] * (seq_t)(primes[i] - 1);
    *phi_out = phi;

    seq_t pos = 0, neg = 0;
    uint64_t subsets = (uint64_t)1 << cnt;
    for (uint64_t mask = 0; mask < subsets; ++mask) {
        uint64_t d = 1;
        int bits = 0;
        for (size_t i = 0; i < cnt; ++i)
            if (mask & ((uint64_t)1 << i)) { d *= primes[i]; ++bits; }
        uint64_t q = half / d;
        if (q == 0) continue;
        seq_t term = (seq_t)d * ((seq_t)q * ((seq_t)q + 1) / 2);
        if (bits & 1) neg += term; else pos += term;
    }
    return pos - neg;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (uint64_t m = 2;; ++m) {
        seq_t phi;
        seq_t s = A341016_half_sum(m, &phi);
        if (s == 0) continue;
        seq_t total = (seq_t)m * phi / 2;
        seq_t big = total - s;
        if (big % s == 0) {
            ++count;
            if (count == n) return (seq_t)m;
        }
    }
}
