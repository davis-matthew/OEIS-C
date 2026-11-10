/*
 * A346615  Moebius transform of A344005.
 * https://oeis.org/A346615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d | n+1} mu((n+1)/d) * A344005(d) (Moebius
 *            transform, offset shifted so a(0) corresponds to index 1)
 * Range:     i8 n <= 79, i16 n <= 79, i32 n <= 79, i64 n <= 79, i128 n <= 79,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 79 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Moebius function mu(m) for m >= 1, by trial division */
static int A346615_mu(uint64_t m)
{
    int res = 1;
    if (m == 1ULL) return 1;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            m /= p;
            if (m % p == 0) return 0;
            res = -res;
        }
    }
    if (m > 1) res = -res;
    return res;
}

static uint64_t A346615_modinv(uint64_t a, uint64_t mod)
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

static uint64_t A346615_crt(const uint64_t *mods, const uint64_t *res, int k)
{
    uint64_t x = 0;
    uint64_t M = 1;
    for (int i = 0; i < k; ++i) {
        uint64_t q = mods[i];
        uint64_t r = res[i] % q;
        uint64_t am = M % q;
        uint64_t inv = A346615_modinv(am, q);
        uint64_t delta = (r + q - (x % q)) % q;
        uint64_t t = (delta * inv) % q;
        x = x + M * t;
        M *= q;
        x %= M;
    }
    return x;
}

static uint64_t A344005_a(uint64_t n64)
{
    if (n64 == 1ULL) return 1;
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
        uint64_t val = A346615_crt(prime_powers, residues, num_factors);
        if (val == 0) val = n64;
        if (val < best) best = val;
        if (best == 1ULL) break;
    }
    return best;
}

static void A346615_term(uint64_t N, uint64_t d, int64_t *sum)
{
    int mu = A346615_mu(N / d);
    if (mu == 0) return;
    int64_t v = (int64_t)A344005_a(d);
    if (mu > 0) *sum += v; else *sum -= v;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n + 1ULL;
    int64_t sum = 0;
    for (uint64_t i = 1; i * i <= N; i++) {
        if (N % i == 0) {
            A346615_term(N, i, &sum);
            uint64_t j = N / i;
            if (j != i) A346615_term(N, j, &sum);
        }
    }
    seq_t r = (seq_t)sum;
    return r;
}
