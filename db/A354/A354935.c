/*
 * A354935  Row 5 of A354940: Numbers k for which A345992(k) = 5, divided by
 *          5.
 * https://oeis.org/A354935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k/5 for the n-th k with A345992(k) = 5; found by search,
 *            testing gcd(5j, m) = 5 with m = A344005(5j) (CRT over
 *            prime-power residues 0 or -1)
 * Range:     u8 n <= 52, u16 n <= 6239, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9863 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static int64_t A354935_inv(int64_t a, int64_t mod) {
    int64_t t = 0, nt = 1, r = mod, nr = a % mod;
    while (nr != 0) {
        int64_t q = r / nr, tmp;
        tmp = t - q * nt; t = nt; nt = tmp;
        tmp = r - q * nr; r = nr; nr = tmp;
    }
    if (t < 0) t += mod;
    return t;
}

static uint64_t A354935_gcd(uint64_t a, uint64_t b) {
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static bool A354935_test(uint64_t N) {
    uint64_t pp[16];
    int k = 0;
    uint64_t t = N;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            uint64_t pe = 1;
            while (t % p == 0) { t /= p; pe *= p; }
            pp[k++] = pe;
        }
    }
    if (t > 1) pp[k++] = t;
    uint64_t best = N;
    uint64_t total = 1ULL << k;
    for (uint64_t mask = 0; mask < total; ++mask) {
        uint64_t r = 0, M = 1;
        for (int i = 0; i < k; ++i) {
            uint64_t q = pp[i];
            uint64_t ai = (mask >> i & 1) ? q - 1 : 0;
            uint64_t inv = (uint64_t)A354935_inv((int64_t)(M % q), (int64_t)q);
            uint64_t diff = (ai + q - r % q) % q;
            uint64_t kk = (diff * inv) % q;
            r += M * kk;
            M *= q;
        }
        uint64_t cand = (r == 0) ? N : r;
        if (cand < best) best = cand;
    }
    return A354935_gcd(N, best) == 5;
}

seq_t a(idx_t n) {
    uint64_t count = 0;
    uint64_t j = 0;
    while (count < (uint64_t)n) {
        ++j;
        uint64_t r = j % 5;
        if (r != 1 && r != 3) continue;
        if (A354935_test(5 * j)) ++count;
    }
    return (seq_t)j;
}
