/*
 * A270265  Numbers that are values of the totient function (A002202) but not
 *          of the reduced totient function (A002174).
 * https://oeis.org/A270265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. a(n) = n-th even v that is a totient value (marked by a
 *            DFS over v = prod p^(k-1)*(p-1)) but not a reduced-totient
 *            value. v is a value of psi = lambda iff, for each odd prime q
 *            with q^e || v, either (q-1) | v or some prime p = q^e*d+1 has d
 *            | v/q^e (the 2-part is always given by lambda(2^...
 * Range:     u8 none, u16 n <= 21, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 54 terms through n = 9880 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static uint8_t *A270265_comp = NULL;   /* odd sieve: comp[i] for 2i+1 */
static uint32_t *A270265_pr = NULL;
static size_t A270265_np = 0;
static uint8_t *A270265_tot = NULL;
static uint64_t A270265_V = 0;          /* current build limit */

static uint64_t *A270265_terms = NULL;
static uint64_t A270265_cnt = 0;
static uint64_t A270265_built = 0;

static bool A270265_isprime(uint64_t x) {
    if (x == 2) return true;
    if (x < 2 || !(x & 1)) return false;
    return !A270265_comp[x >> 1];
}

static void A270265_dfs(uint64_t t, size_t i) {
    uint64_t V = A270265_V;
    for (; i < A270265_np; i++) {
        uint64_t p = A270265_pr[i];
        uint64_t tt = t * (p - 1);
        if (tt > V) break;
        for (;;) {
            A270265_tot[tt >> 3] |= (uint8_t)(1u << (tt & 7));
            if (i + 1 < A270265_np && tt <= V / ((uint64_t)A270265_pr[i + 1] - 1))
                A270265_dfs(tt, i + 1);
            if (tt > V / p) break;
            tt *= p;
        }
    }
}

static uint64_t A270265_fp[40];
static int A270265_fe[40];
static int A270265_nf;

static bool A270265_divchk(int idx, uint64_t d, uint64_t qe) {
    if (idx == A270265_nf) return A270265_isprime(qe * d + 1);
    for (int k = 0; k <= A270265_fe[idx]; k++) {
        if (A270265_divchk(idx + 1, d, qe)) return true;
        d *= A270265_fp[idx];
    }
    return false;
}

static void A270265_build(uint64_t V) {
    uint64_t N = V + 1;
    size_t hs = (size_t)(N / 2 + 1);
    A270265_V = V;
    A270265_comp = (uint8_t *)calloc(hs, 1);
    for (uint64_t i = 1; (2 * i + 1) * (2 * i + 1) <= N; i++) {
        if (A270265_comp[i]) continue;
        uint64_t p = 2 * i + 1;
        for (uint64_t j = (p * p) >> 1; j < hs; j += p) A270265_comp[j] = 1;
    }
    size_t cnt = 1;
    for (uint64_t i = 1; 2 * i + 1 <= N; i++) if (!A270265_comp[i]) cnt++;
    A270265_pr = (uint32_t *)malloc(sizeof(uint32_t) * (cnt + 1));
    A270265_np = 0;
    A270265_pr[A270265_np++] = 2;
    for (uint64_t i = 1; 2 * i + 1 <= N; i++)
        if (!A270265_comp[i]) A270265_pr[A270265_np++] = (uint32_t)(2 * i + 1);

    A270265_tot = (uint8_t *)calloc((size_t)(V / 8 + 2), 1);
    uint8_t *fail = (uint8_t *)calloc((size_t)(V / 8 + 2), 1);
    A270265_tot[0] |= 2; /* 1 */
    A270265_dfs(1, 0);

    for (size_t k = 2; k < A270265_np; k++) { /* odd primes q >= 5 */
        uint64_t q = A270265_pr[k];
        if (q * q > V) break;
        uint64_t step = 2 * q * q;
        for (uint64_t v = step; v <= V; v += step) {
            if (!(A270265_tot[v >> 3] & (1u << (v & 7)))) continue;
            if (fail[v >> 3] & (1u << (v & 7))) continue;
            if (v % (q - 1) == 0) continue;
            uint64_t w = v / (q * q), qe = q * q;
            while (w % q == 0) { w /= q; qe *= q; }
            /* factor w */
            A270265_nf = 0;
            uint64_t r = w;
            for (size_t j = 0; j < A270265_np; j++) {
                uint64_t p = A270265_pr[j];
                if (p * p > r) break;
                if (r % p == 0) {
                    int e = 0;
                    while (r % p == 0) { r /= p; e++; }
                    A270265_fp[A270265_nf] = p;
                    A270265_fe[A270265_nf] = e;
                    A270265_nf++;
                }
            }
            if (r > 1) { A270265_fp[A270265_nf] = r; A270265_fe[A270265_nf] = 1; A270265_nf++; }
            if (!A270265_divchk(0, 1, qe))
                fail[v >> 3] |= (uint8_t)(1u << (v & 7));
        }
    }

    free(A270265_terms);
    uint64_t cap = 1024;
    A270265_terms = (uint64_t *)malloc(sizeof(uint64_t) * cap);
    A270265_cnt = 0;
    for (uint64_t v = 2; v <= V; v += 2) {
        if (!(A270265_tot[v >> 3] & (1u << (v & 7)))) continue;
        if (!(fail[v >> 3] & (1u << (v & 7)))) continue;
        if (A270265_cnt == cap) {
            cap *= 2;
            A270265_terms = (uint64_t *)realloc(A270265_terms, sizeof(uint64_t) * cap);
        }
        A270265_terms[A270265_cnt++] = v;
    }
    A270265_built = V;
    free(fail);
    free(A270265_tot); A270265_tot = NULL;
    free(A270265_pr); A270265_pr = NULL;
    free(A270265_comp); A270265_comp = NULL;
}

seq_t a(idx_t n) {
    if (A270265_built == 0 || A270265_cnt < (uint64_t)n) {
        uint64_t V = (uint64_t)n * 4000;
        if (V < ((uint64_t)1 << 20)) V = (uint64_t)1 << 20;
        if (V <= A270265_built) V = A270265_built + A270265_built / 2;
        for (;;) {
            A270265_build(V);
            if (A270265_cnt >= (uint64_t)n) break;
            V = V + V / 2;
        }
    }
    return (seq_t)A270265_terms[n - 1];
}
