/*
 * A354919  Positions of odd terms in A344005.
 * https://oeis.org/A354919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A344005(k) is odd (search over k,
 *            computing A344005 via CRT over prime-power splittings of k)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A344005_modinv(uint64_t a, uint64_t mod)
{
    int64_t t = 0, newt = 1;
    int64_t r = (int64_t)mod, newr = (int64_t)(a % mod);
    while (newr != 0) {
        uint64_t q = (uint64_t)(r) / (uint64_t)(newr);
        int64_t tmp = t - (int64_t)q * newt;
        t = newt; newt = tmp;
        tmp = r - (int64_t)q * newr;
        r = newr; newr = tmp;
    }
    if (r > 1) return 0;
    if (t < 0) t += (int64_t)mod;
    return (uint64_t)t;
}

static uint64_t A344005_crt(const uint64_t *mods, const uint64_t *res, int k)
{
    uint64_t x = 0;
    uint64_t M = 1;
    for (int i = 0; i < k; ++i) {
        uint64_t q = mods[i];
        uint64_t r = res[i] % q;
        uint64_t a = M % q;
        uint64_t inv = A344005_modinv(a, q);
        uint64_t delta = (r + q - (x % q)) % q;
        uint64_t t = (delta * inv) % q;
        x = x + M * t;
        M *= q;
        x %= M;
    }
    return x;
}

static uint64_t A344005_val(uint64_t n64)
{
    if (n64 == 1) return 1;
    uint64_t prime_powers[64];
    int num_factors = 0;
    uint64_t temp = n64;

    if ((temp & 1ULL) == 0) {
        uint64_t pw = 1;
        while ((temp & 1ULL) == 0) {
            temp >>= 1;
            pw <<= 1;
        }
        prime_powers[num_factors++] = pw;
    }
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            uint64_t pw = 1;
            while (temp % p == 0) {
                temp /= p;
                pw *= p;
            }
            prime_powers[num_factors++] = pw;
        }
    }
    if (temp > 1)
        prime_powers[num_factors++] = temp;

    if (num_factors == 1)
        return n64 - 1ULL;

    uint64_t best = (uint64_t)-1;
    uint64_t subsets = 1ULL << num_factors;
    for (uint64_t mask = 0; mask < subsets; ++mask) {
        uint64_t residues[64];
        for (int i = 0; i < num_factors; ++i) {
            uint64_t q = prime_powers[i];
            residues[i] = (mask & (1ULL << i)) ? (q - 1ULL) : 0ULL;
        }
        uint64_t val = A344005_crt(prime_powers, residues, num_factors);
        if (val == 0) val = n64;
        if (val < best) best = val;
        if (best == 1ULL) break;
    }
    return best;
}

seq_t a(idx_t n)
{
    uint64_t count = 0;
    uint64_t k = 0;
    for (;;) {
        ++k;
        if (A344005_val(k) & 1ULL) {
            ++count;
            if (count == (uint64_t)n) return (seq_t)k;
        }
    }
}
