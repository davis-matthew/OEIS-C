/*
 * A345444  a(n) = A344005(2*n+1).
 * https://oeis.org/A345444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A344005(2*n+1) = smallest positive m with (2n+1) |
 *            m*(m+1); computed by CRT over residues 0 or -1 mod each prime
 *            power of 2n+1
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 72 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

typedef unsigned __int128 u128_t345444;
typedef __int128 i128_t345444;

static uint64_t A345444_inv(uint64_t a, uint64_t mod)
{
    i128_t345444 t = 0, nt = 1;
    i128_t345444 r = (i128_t345444)mod, nr = (i128_t345444)(a % mod);
    while (nr != 0) {
        i128_t345444 q = r / nr;
        i128_t345444 tmp = t - q * nt; t = nt; nt = tmp;
        tmp = r - q * nr; r = nr; nr = tmp;
    }
    if (t < 0) t += (i128_t345444)mod;
    return (uint64_t)t;
}

static uint64_t A345444_crt(const uint64_t *mods, const uint64_t *res, int k)
{
    uint64_t x = 0, M = 1;
    for (int i = 0; i < k; ++i) {
        uint64_t q = mods[i];
        uint64_t r = res[i] % q;
        uint64_t inv = A345444_inv(M % q, q);
        uint64_t delta = (uint64_t)(((u128_t345444)r + q - (x % q)) % q);
        uint64_t t = (uint64_t)(((u128_t345444)delta * inv) % q);
        x = x + M * t;
        M *= q;
    }
    return x;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n * 2u + 1u;
    if (N == 1) return (seq_t)1;

    uint64_t pp[32];
    int k = 0;
    uint64_t temp = N;
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            uint64_t pw = 1;
            while (temp % p == 0) { temp /= p; pw *= p; }
            pp[k++] = pw;
        }
    }
    if (temp > 1) pp[k++] = temp;

    uint64_t best = N - 1;
    uint64_t subsets = (uint64_t)1 << k;
    for (uint64_t mask = 0; mask < subsets; ++mask) {
        uint64_t res[32];
        for (int i = 0; i < k; ++i)
            res[i] = (mask & ((uint64_t)1 << i)) ? pp[i] - 1 : 0;
        uint64_t v = A345444_crt(pp, res, k);
        if (v == 0) v = N;
        if (v < best) best = v;
    }
    return (seq_t)best;
}
