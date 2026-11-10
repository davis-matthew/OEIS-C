/*
 * A389603  Smallest k for which A389362(k) = n.
 * https://oeis.org/A389603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{ k : A389362(k) = n }, found by search: a depth-first
 *            walk over the prime factorizations of every k <= B with tau(k)
 *            >= n (this is enough, because A389362(k) <= tau(k) - prod e_i).
 *            B is doubled until the smallest k found is certain to be the
 *            minimum
 * Range:     u8 n <= 14, u16 n <= 63, u32 n <= 800, u64 n <= 800,
 *            u128 n <= 800, big any n
 * Validated: standard, pass, 60 terms through n = 512 (2026-11-10)
 * Helpers:   mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mulmod.h"

#define A389603_CMAX 131072

static uint64_t A389603_best[A389603_CMAX];
static uint64_t A389603_ver[A389603_CMAX];

static uint64_t *A389603_pr = NULL;
static size_t A389603_np = 0;
static uint64_t A389603_plim = 0;

static uint8_t *A389603_mark = NULL;
static uint64_t *A389603_res = NULL;
static uint64_t A389603_bufcap = 0;

static uint64_t A389603_B, A389603_T;
static int A389603_fail;
static uint64_t A389603_p[64];
static unsigned A389603_e[64];

static int A389603_extend(void)
{
    uint64_t L = A389603_plim ? A389603_plim * 2 : 4096;
    if (L > ((uint64_t)1 << 30)) return 0;
    unsigned char *s = (unsigned char *)calloc((size_t)L + 1, 1);
    if (!s) return 0;
    size_t cnt = 0;
    for (uint64_t i = 2; i <= L; i++)
        if (!s[i]) {
            cnt++;
            for (uint64_t j = i * i; j <= L; j += i) s[j] = 1;
        }
    uint64_t *np = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!np) { free(s); return 0; }
    size_t c = 0;
    for (uint64_t i = 2; i <= L; i++)
        if (!s[i]) np[c++] = i;
    free(s);
    free(A389603_pr);
    A389603_pr = np;
    A389603_np = cnt;
    A389603_plim = L;
    return 1;
}

static inline uint64_t A389603_mulm(uint64_t x, uint64_t y, uint64_t P)
{
    if (P < ((uint64_t)1 << 32)) return (x * y) % P;
    return mulmod(x, y, P);
}

/* A389362 of k = prod p_i^e_i (r >= 1 primes), tau = number of divisors */
static uint64_t A389603_count(int r, uint64_t tau)
{
    if (tau > A389603_bufcap) {
        uint64_t nc = tau * 2;
        uint8_t *m2 = (uint8_t *)realloc(A389603_mark, (size_t)nc);
        if (!m2) return 0;
        A389603_mark = m2;
        uint64_t *r2 = (uint64_t *)realloc(A389603_res, (size_t)nc * sizeof(uint64_t));
        if (!r2) return 0;
        A389603_res = r2;
        A389603_bufcap = nc;
    }
    uint8_t *mark = A389603_mark;
    uint64_t *res = A389603_res;
    memset(mark, 0, (size_t)tau);
    uint64_t S = 1;
    for (int j = 0; j < r; j++) {
        uint64_t P = A389603_p[j];
        uint64_t ej = A389603_e[j];
        uint64_t block = S * (ej + 1);
        uint64_t nhi = tau / block;
        if (P <= 3) {
            /* every divisor coprime to 2 or 3 is +-1 mod it */
            for (uint64_t hi = 0; hi < nhi; hi++) {
                uint64_t base = hi * block;
                for (uint64_t lo = 0; lo < S; lo++) mark[base + lo] = 1;
            }
        } else {
            uint64_t nr = 1;
            res[0] = 1;
            for (int i = 0; i < r; i++) {
                if (i == j) continue;
                uint64_t pm = A389603_p[i] % P;
                uint64_t len = nr, mult = 1;
                for (unsigned f = 1; f <= A389603_e[i]; f++) {
                    mult = A389603_mulm(mult, pm, P);
                    for (uint64_t t = 0; t < len; t++)
                        res[nr++] = A389603_mulm(res[t], mult, P);
                }
            }
            for (uint64_t hi = 0; hi < nhi; hi++) {
                const uint64_t *rr = res + hi * S;
                uint8_t *mm = mark + hi * block;
                for (uint64_t lo = 0; lo < S; lo++) {
                    uint64_t v = rr[lo];
                    if (v == 1 || v == P - 1) mm[lo] = 1;
                }
            }
        }
        S = block;
    }
    uint64_t c = 0;
    for (uint64_t i = 0; i < tau; i++) c += mark[i];
    return c;
}

static void A389603_dfs(size_t qi, int r, uint64_t m, uint64_t tau, uint64_t prode)
{
    uint64_t ub = (r == 0) ? 0 : tau - prode;
    if (ub >= A389603_T) {
        uint64_t c = (r == 0) ? 0 : A389603_count(r, tau);
        if (c < A389603_CMAX && (A389603_best[c] == 0 || m < A389603_best[c]))
            A389603_best[c] = m;
    }
    uint64_t R = A389603_B / m;
    for (size_t q = qi;; q++) {
        if (q >= A389603_np) {
            if (A389603_plim >= R) break;
            if (!A389603_extend()) { A389603_fail = 1; return; }
            if (q >= A389603_np) break;
        }
        uint64_t p = A389603_pr[q];
        if (p > R) break;
        unsigned L = 0;
        uint64_t x = 1;
        while (x <= R / p) { x *= p; L++; }
        uint64_t bound = tau;
        for (unsigned t = 0; t < L && bound < A389603_T; t++) bound *= 2;
        if (bound < A389603_T) break;
        uint64_t mm = m;
        A389603_p[r] = p;
        for (unsigned e = 1; e <= L; e++) {
            mm *= p;
            A389603_e[r] = e;
            A389603_dfs(q + 1, r + 1, mm, tau * (e + 1), prode * e);
            if (A389603_fail) return;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n >= A389603_CMAX) return (seq_t)0;
    for (;;) {
        if (A389603_best[n] && A389603_best[n] <= A389603_ver[n])
            return (seq_t)A389603_best[n];
        uint64_t B = A389603_ver[n] < 8 ? 16 : A389603_ver[n] * 2;
        if (B > ((uint64_t)1 << 62)) return (seq_t)0;
        A389603_B = B;
        A389603_T = (uint64_t)n;
        A389603_fail = 0;
        A389603_dfs(0, 0, 1, 1, 1);
        if (A389603_fail) return (seq_t)0;
        for (uint64_t c = (uint64_t)n; c < A389603_CMAX; c++)
            if (A389603_ver[c] < B) A389603_ver[c] = B;
    }
}
