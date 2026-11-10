/*
 * A354877  Positions of zeros in A354875, which is the Dirichlet inverse of
 *          A344005.
 * https://oeis.org/A354877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search/sieve: b = Dirichlet inverse of A344005, b(1) = 1, b(n) =
 *            -Sum_{d|n, d<n} A344005(n/d)*b(d); a(n) = n-th k with b(k) = 0.
 *            b is tracked modulo 2^64 and modulo 2^61-1 to detect zeros
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-10)
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
        uint64_t aa = M % q;
        uint64_t inv = A344005_modinv(aa, q);
        uint64_t delta = (r + q - (x % q)) % q;
        uint64_t t = (uint64_t)(((unsigned __int128)delta * inv) % q);
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

#define P61 ((1ULL << 61) - 1ULL)

static uint64_t mulmod61(uint64_t x, uint64_t y)
{
    unsigned __int128 z = (unsigned __int128)x * y;
    uint64_t lo = (uint64_t)(z & P61);
    uint64_t hi = (uint64_t)(z >> 61);
    uint64_t s = lo + hi;
    if (s >= P61) s -= P61;
    return s;
}

static uint64_t *g_zeros = NULL;
static size_t g_count = 0;
static uint64_t g_limit = 0;

static void compute_zeros(uint64_t N)
{
    uint64_t *A = (uint64_t *)calloc(N + 1, sizeof(uint64_t));
    uint64_t *b1 = (uint64_t *)calloc(N + 1, sizeof(uint64_t));
    uint64_t *b2 = (uint64_t *)calloc(N + 1, sizeof(uint64_t));
    size_t cap = 64;
    uint64_t *z = (uint64_t *)malloc(cap * sizeof(uint64_t));
    size_t cnt = 0;
    for (uint64_t m = 1; m <= N; ++m) A[m] = A344005_val(m);
    b1[1] = 1; b2[1] = 1;
    for (uint64_t d = 1; d <= N; ++d) {
        if (d > 1) {
            b1[d] = (uint64_t)0 - b1[d];
            b2[d] = b2[d] ? P61 - b2[d] : 0;
        }
        if (b1[d] == 0 && b2[d] == 0) {
            if (cnt == cap) { cap *= 2; z = (uint64_t *)realloc(z, cap * sizeof(uint64_t)); }
            z[cnt++] = d;
            continue;
        }
        uint64_t x1 = b1[d], x2 = b2[d];
        for (uint64_t k = 2, m = 2 * d; m <= N; ++k, m += d) {
            b1[m] += x1 * A[k];
            uint64_t s = b2[m] + mulmod61(x2, A[k] % P61);
            if (s >= P61) s -= P61;
            b2[m] = s;
        }
    }
    free(A); free(b1); free(b2);
    free(g_zeros);
    g_zeros = z;
    g_count = cnt;
    g_limit = N;
}

seq_t a(idx_t n)
{
    while (g_count < (size_t)n) {
        uint64_t N = g_limit ? g_limit * 2 : (1ULL << 18);
        compute_zeros(N);
    }
    return (seq_t)g_zeros[(size_t)(n - 1)];
}
