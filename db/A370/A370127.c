/*
 * A370127  Numbers k such that (A276086(k)/s)^s < k^(s-1), where A276086 is
 *          the primorial base exp-function, and s = bigomega(k).
 * https://oeis.org/A370127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search, in increasing order, over k written in primorial base; a
 *            branch is cut as soon as A276086(k) >= log2(k)*k, which already
 *            rules it out because s = bigomega(k) <= log2(k). Each remaining
 *            k is tested exactly: (A276086(k))^s < s^s * k^(s-1), with s =
 *            A001222(k)
 * Range:     u8 n <= 8, u16 n <= 134, u32 n <= 5204, u64 n <= 5204,
 *            u128 n <= 5204, big any n
 * Validated: standard, pass, 69 terms through n = 4253 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define A370127_LIMBS 160
#define A370127_NP 16

static const uint64_t A370127_PR[A370127_NP] =
    {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};
static uint64_t A370127_PRIM[A370127_NP + 1];

static uint32_t *A370127_sp = NULL;
static size_t A370127_nsp = 0;

static void A370127_init(void)
{
    static bool done = false;
    if (done) return;
    done = true;
    A370127_PRIM[0] = 1;
    for (int i = 1; i <= A370127_NP; ++i)
        A370127_PRIM[i] = A370127_PRIM[i-1] * A370127_PR[i-1];
    const uint32_t LIM = 65536;
    char *c = (char *)calloc(LIM + 1, 1);
    if (!c) abort();
    A370127_sp = (uint32_t *)malloc(sizeof(uint32_t) * 7000);
    if (!A370127_sp) abort();
    for (uint32_t i = 2; i <= LIM; ++i) {
        if (c[i]) continue;
        A370127_sp[A370127_nsp++] = i;
        for (uint64_t j = (uint64_t)i * i; j <= LIM; j += i) c[j] = 1;
    }
    free(c);
}

static int A370127_bigomega(uint64_t k)
{
    int c = 0;
    size_t i;
    for (i = 0; i < A370127_nsp; ++i) {
        uint64_t p = A370127_sp[i];
        if (p * p > k) break;
        while (k % p == 0) { c++; k /= p; }
    }
    if (i == A370127_nsp) {
        for (uint64_t p = 65537; p <= k / p; p += 2)
            while (k % p == 0) { c++; k /= p; }
    }
    if (k > 1) c++;
    return c;
}

static int A370127_bitlen(uint64_t x)
{
    int b = 0;
    while (x) { b++; x >>= 1; }
    return b;
}

/* multiply little-endian 32-bit limb bignum by m (< 2^64) */
static int A370127_mul(uint32_t *x, int len, uint64_t m)
{
    unsigned __int128 carry = 0;
    for (int i = 0; i < len; ++i) {
        unsigned __int128 t = (unsigned __int128)x[i] * m + carry;
        x[i] = (uint32_t)t;
        carry = t >> 32;
    }
    while (carry && len < A370127_LIMBS) {
        x[len++] = (uint32_t)carry;
        carry >>= 32;
    }
    return len;
}

static int A370127_cmp(const uint32_t *x, int lx, const uint32_t *y, int ly)
{
    while (lx > 0 && x[lx-1] == 0) lx--;
    while (ly > 0 && y[ly-1] == 0) ly--;
    if (lx != ly) return lx < ly ? -1 : 1;
    for (int i = lx - 1; i >= 0; --i)
        if (x[i] != y[i]) return x[i] < y[i] ? -1 : 1;
    return 0;
}

static bool A370127_test(uint64_t k, uint64_t A)
{
    int s = A370127_bigomega(k);
    if (s < 2) return false;
    if ((unsigned __int128)A >= (unsigned __int128)s * k) return false;
    uint32_t L[A370127_LIMBS], R[A370127_LIMBS];
    int ll = 1, lr = 1;
    L[0] = 1; R[0] = 1;
    for (int i = 0; i < s; ++i) ll = A370127_mul(L, ll, A);
    for (int i = 0; i < s; ++i) lr = A370127_mul(R, lr, (uint64_t)s);
    for (int i = 0; i < s - 1; ++i) lr = A370127_mul(R, lr, k);
    return A370127_cmp(L, ll, R, lr) < 0;
}

static uint64_t *A370127_terms = NULL;
static size_t A370127_cnt = 0, A370127_cap = 0;

static void A370127_push(uint64_t k)
{
    if (A370127_cnt == A370127_cap) {
        A370127_cap = A370127_cap ? A370127_cap * 2 : 1024;
        A370127_terms = (uint64_t *)realloc(A370127_terms,
                                            A370127_cap * sizeof(uint64_t));
        if (!A370127_terms) abort();
    }
    A370127_terms[A370127_cnt++] = k;
}

/* choose digits at positions i..0; digits ascending gives k ascending */
static void A370127_dfs(int i, uint64_t partial, unsigned __int128 A)
{
    if (i < 0) {
        if (A370127_test(partial, (uint64_t)A)) A370127_push(partial);
        return;
    }
    uint64_t p = A370127_PR[i], w = A370127_PRIM[i];
    unsigned __int128 Ad = A;
    for (uint64_t d = 0; d < p; ++d) {
        uint64_t lo = partial + d * w;
        uint64_t up = lo + w;
        unsigned __int128 bound =
            (unsigned __int128)A370127_bitlen(up) * up;
        if (Ad >= bound) break;
        A370127_dfs(i - 1, lo, Ad);
        Ad *= p;
    }
}

seq_t a(idx_t n)
{
    static int next_block = 1;
    A370127_init();
    while (A370127_cnt < (size_t)n && next_block < A370127_NP) {
        int m = next_block;
        uint64_t p = A370127_PR[m], w = A370127_PRIM[m];
        unsigned __int128 Ad = p;
        for (uint64_t d = 1; d < p; ++d) {
            uint64_t lo = d * w;
            uint64_t up = lo + w;
            unsigned __int128 bound =
                (unsigned __int128)A370127_bitlen(up) * up;
            if (Ad >= bound) break;
            A370127_dfs(m - 1, lo, Ad);
            Ad *= p;
        }
        next_block++;
    }
    if (A370127_cnt < (size_t)n) return 0;
    return (seq_t)A370127_terms[(size_t)n - 1];
}
