/*
 * A354994  Numbers k such that A344005(d) < A344005(k) for all proper
 *          divisors d of k.
 * https://oeis.org/A354994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search/sieve. a(n) = n-th k such that A344005(d) < A344005(k)
 *            for all proper divisors d of k; A344005 computed via CRT over
 *            prime-power factors, limit doubled until n terms found
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: partial, pass, 77 terms through n = 77 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

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
    uint64_t x = 0, M = 1;
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
    uint64_t pp[64];
    int nf = 0;
    uint64_t temp = n64;
    if ((temp & 1ULL) == 0) {
        uint64_t pw = 1;
        while ((temp & 1ULL) == 0) { temp >>= 1; pw <<= 1; }
        pp[nf++] = pw;
    }
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            uint64_t pw = 1;
            while (temp % p == 0) { temp /= p; pw *= p; }
            pp[nf++] = pw;
        }
    }
    if (temp > 1) pp[nf++] = temp;
    if (nf == 1) return n64 - 1ULL;
    uint64_t best = (uint64_t)-1;
    uint64_t subsets = 1ULL << nf;
    for (uint64_t mask = 0; mask < subsets; ++mask) {
        uint64_t residues[64];
        for (int i = 0; i < nf; ++i) {
            uint64_t q = pp[i];
            residues[i] = (mask & (1ULL << i)) ? (q - 1ULL) : 0ULL;
        }
        uint64_t v = A344005_crt(pp, residues, nf);
        if (v == 0) v = n64;
        if (v < best) best = v;
        if (best == 1ULL) break;
    }
    return best;
}

seq_t a(idx_t n)
{
    uint64_t L = 1024;
    for (;;) {
        uint64_t *A = (uint64_t *)malloc((L + 1) * sizeof(uint64_t));
        unsigned char *bad = (unsigned char *)calloc(L + 1, 1);
        if (!A || !bad) { free(A); free(bad); return 0; }
        for (uint64_t k = 1; k <= L; ++k) A[k] = A344005_val(k);
        for (uint64_t d = 1; 2 * d <= L; ++d)
            for (uint64_t k = 2 * d; k <= L; k += d)
                if (A[d] >= A[k]) bad[k] = 1;
        uint64_t cnt = 0, found = 0;
        for (uint64_t k = 1; k <= L; ++k) {
            if (!bad[k]) {
                ++cnt;
                if (cnt == (uint64_t)n) { found = k; break; }
            }
        }
        free(A);
        free(bad);
        if (found) return (seq_t)found;
        L *= 2;
    }
}
