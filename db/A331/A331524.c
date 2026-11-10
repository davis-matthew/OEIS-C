/*
 * A331524  a(n) is the least positive k such that A006686(n) - k^8 is an
 *          eighth power.
 * https://oeis.org/A331524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = x, where A006686(n) = x^8 + y^8 with 1 <= x <= y (least k
 *            with A006686(n) - k^8 an eighth power); found by search over x,y
 * Range:     u8 n <= 1674, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9736 (2026-11-08)
 * Helpers:   gcd_i64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_i64.h"

typedef unsigned __int128 A331524_u128;

typedef struct { A331524_u128 v; long x; } A331524_ent;

static A331524_ent *A331524_cache = NULL;
static idx_t A331524_len = 0;
static long A331524_M = 0;
static int *A331524_small = NULL;
static int A331524_nsmall = 0;

static int A331524_bitlen(A331524_u128 v) {
    int b = 0;
    while (v) { b++; v >>= 1; }
    return b;
}

static A331524_u128 A331524_mulmod(A331524_u128 a, A331524_u128 b, A331524_u128 m, int s) {
    if ((m >> 64) == 0) return (a * b) % m;
    A331524_u128 mask = (((A331524_u128)1) << s) - 1;
    int nb = A331524_bitlen(b);
    int c = (nb + s - 1) / s;
    A331524_u128 r = 0;
    int i;
    for (i = c - 1; i >= 0; i--) {
        r = (r << s) % m;
        r = (r + a * ((b >> (i * s)) & mask)) % m;
    }
    return r;
}

static A331524_u128 A331524_powmod(A331524_u128 b, A331524_u128 e, A331524_u128 m, int s) {
    A331524_u128 r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = A331524_mulmod(r, b, m, s);
        e >>= 1;
        if (e) b = A331524_mulmod(b, b, m, s);
    }
    return r;
}

static bool A331524_mr(A331524_u128 n) {
    static const unsigned bases[20] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
                                       31, 37, 41, 43, 47, 53, 59, 61, 67, 71};
    int i, j, k, nbases, s;
    A331524_u128 d;
    if (n < 2) return false;
    for (i = 0; i < 20; i++) {
        if (n == bases[i]) return true;
        if (n % bases[i] == 0) return false;
    }
    s = 127 - A331524_bitlen(n);
    if (s < 1) s = 1;
    d = n - 1;
    k = 0;
    while ((d & 1) == 0) { d >>= 1; k++; }
    nbases = (n < (A331524_u128)3317044064679887385ULL * 1000000ULL) ? 13 : 20;
    for (i = 0; i < nbases; i++) {
        A331524_u128 x = A331524_powmod(bases[i], d, n, s);
        if (x == 1 || x == n - 1) continue;
        for (j = 1; j < k; j++) {
            x = A331524_mulmod(x, x, n, s);
            if (x == n - 1) break;
        }
        if (j >= k) return false;
    }
    return true;
}

static void A331524_init_small(void) {
    int lim = 20000, i, j;
    char *comp;
    if (A331524_small) return;
    comp = (char *)calloc((size_t)lim + 1, 1);
    A331524_small = (int *)malloc(sizeof(int) * (size_t)lim);
    for (i = 2; i <= lim; i++) {
        if (comp[i]) continue;
        if (i % 16 == 1) A331524_small[A331524_nsmall++] = i;
        for (j = i * 2; j <= lim; j += i) comp[j] = 1;
    }
    free(comp);
}

static bool A331524_isprime(A331524_u128 v) {
    int i;
    if (v == 2) return true;
    if (v < 2 || (v & 1) == 0) return false;
    for (i = 0; i < A331524_nsmall; i++) {
        A331524_u128 p = (A331524_u128)A331524_small[i];
        if (p * p > v) return true;
        if (v % p == 0) return v == p;
    }
    return A331524_mr(v);
}

static int A331524_cmp(const void *a, const void *b) {
    A331524_u128 va = ((const A331524_ent *)a)->v, vb = ((const A331524_ent *)b)->v;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

static void A331524_build(long M) {
    A331524_u128 *p8 = (A331524_u128 *)malloc(sizeof(A331524_u128) * (size_t)(M + 1));
    A331524_ent *res;
    size_t cap = 1024, cnt = 0;
    A331524_u128 L;
    long x, y;
    int i;
    for (x = 0; x <= M; x++) {
        A331524_u128 v = 1;
        for (i = 0; i < 8; i++) v *= (A331524_u128)x;
        p8[x] = v;
    }
    L = p8[M];
    res = (A331524_ent *)malloc(sizeof(A331524_ent) * cap);
    for (y = 1; y <= M; y++) {
        for (x = 0; x <= y; x++) {
            A331524_u128 v;
            if (!(x == 1 && y == 1)) {
                if (((x + y) & 1) == 0) continue;
                if (gcd_i64(x, y) != 1) continue;
            }
            v = p8[x] + p8[y];
            if (v > L) break;
            if (!A331524_isprime(v)) continue;
            if (cnt == cap) {
                cap *= 2;
                res = (A331524_ent *)realloc(res, sizeof(A331524_ent) * cap);
            }
            res[cnt].v = v;
            res[cnt].x = x;
            cnt++;
        }
    }
    free(p8);
    qsort(res, cnt, sizeof(A331524_ent), A331524_cmp);
    if (A331524_cache) free(A331524_cache);
    A331524_cache = res;
    A331524_len = (idx_t)cnt;
    A331524_M = M;
}

seq_t a(idx_t n) {
    A331524_init_small();
    if (A331524_M == 0) A331524_build(32);
    while (A331524_len < n) A331524_build(A331524_M * 2);
    return (seq_t)(unsigned long long)A331524_cache[n - 1].x;
}
