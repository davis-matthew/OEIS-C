/*
 * A334104  Numbers m for which A329697(m) = 4.
 * https://oeis.org/A334104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number m with A329697(m) = 4; computed by a reverse
 *            search: the odd parts o = q*p (p prime, p >= lpf(q)) whose step
 *            count is k are built from those with count k-1, then multiplied
 *            by powers of 2 and sorted
 * Range:     u8 n <= 79, u16 n <= 1790, u32 n <= 12193, u64 n <= 12193,
 *            u128 n <= 12193, big any n
 * Validated: standard, pass, 84 terms through n = 9811 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    uint64_t v;
    uint64_t pr[16];
    uint8_t ex[16];
    int nf;
} A334104_el;

typedef struct {
    A334104_el *a;
    size_t n, cap;
} A334104_vec;

static uint64_t A334104_mulmod(uint64_t a, uint64_t b, uint64_t m)
{
    return (uint64_t)((unsigned __int128)a * b % m);
}

static uint64_t A334104_powmod(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    b %= m;
    while (e) {
        if (e & 1) r = A334104_mulmod(r, b, m);
        b = A334104_mulmod(b, b, m);
        e >>= 1;
    }
    return r;
}

static bool A334104_isprime(uint64_t n)
{
    static const uint64_t bs[12] = {2,3,5,7,11,13,17,19,23,29,31,37};
    if (n < 2) return false;
    for (int i = 0; i < 12; i++) {
        if (n == bs[i]) return true;
        if (n % bs[i] == 0) return false;
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (int i = 0; i < 12; i++) {
        uint64_t x = A334104_powmod(bs[i], d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (int r = 1; r < s; r++) {
            x = A334104_mulmod(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

static void A334104_push(A334104_vec *v, const A334104_el *e)
{
    if (v->n == v->cap) {
        v->cap = v->cap ? v->cap * 2 : 1024;
        v->a = (A334104_el *)realloc(v->a, v->cap * sizeof(A334104_el));
    }
    v->a[v->n++] = *e;
}

static void A334104_rec(const A334104_el *t, int idx, uint64_t d, uint8_t *cur,
                        A334104_vec *out, uint64_t L)
{
    if (idx == t->nf) {
        uint64_t q = t->v / d;
        uint64_t lpf = 1;
        for (int i = t->nf - 1; i >= 0; i--) {
            if (cur[i] < t->ex[i]) { lpf = t->pr[i]; break; }
        }
        uint64_t x = d * 2;
        while (x <= L) {
            uint64_t p = x + 1;
            if (q > L / p) break;
            if (p >= lpf && A334104_isprime(p)) {
                A334104_el e;
                e.nf = 0;
                for (int i = 0; i < t->nf; i++) {
                    int rem = t->ex[i] - cur[i];
                    if (rem > 0) {
                        e.pr[e.nf] = t->pr[i];
                        e.ex[e.nf] = (uint8_t)rem;
                        e.nf++;
                    }
                }
                if (e.nf > 0 && e.pr[e.nf - 1] == p) {
                    e.ex[e.nf - 1]++;
                } else {
                    e.pr[e.nf] = p;
                    e.ex[e.nf] = 1;
                    e.nf++;
                }
                e.v = q * p;
                A334104_push(out, &e);
            }
            x <<= 1;
        }
        return;
    }
    uint64_t dd = d;
    for (int k = 0; k <= t->ex[idx]; k++) {
        cur[idx] = (uint8_t)k;
        A334104_rec(t, idx + 1, dd, cur, out, L);
        dd *= t->pr[idx];
    }
    cur[idx] = 0;
}

static int A334104_cmp(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a, y = *(const uint64_t *)b;
    return x < y ? -1 : (x > y ? 1 : 0);
}

static uint64_t *A334104_cache = 0;
static size_t A334104_cnt = 0;

static void A334104_build(uint64_t L)
{
    A334104_vec cur_v, next_v;
    cur_v.a = 0; cur_v.n = 0; cur_v.cap = 0;
    A334104_el one;
    one.v = 1; one.nf = 0;
    A334104_push(&cur_v, &one);
    for (int lvl = 1; lvl <= 4; lvl++) {
        next_v.a = 0; next_v.n = 0; next_v.cap = 0;
        for (size_t i = 0; i < cur_v.n; i++) {
            uint8_t cur[16];
            for (int k = 0; k < 16; k++) cur[k] = 0;
            A334104_rec(&cur_v.a[i], 0, 1, cur, &next_v, L);
        }
        free(cur_v.a);
        cur_v = next_v;
    }
    size_t cap = 1024, n = 0;
    uint64_t *res = (uint64_t *)malloc(cap * sizeof(uint64_t));
    for (size_t i = 0; i < cur_v.n; i++) {
        uint64_t o = cur_v.a[i].v;
        while (1) {
            if (n == cap) {
                cap *= 2;
                res = (uint64_t *)realloc(res, cap * sizeof(uint64_t));
            }
            res[n++] = o;
            if (o > L / 2) break;
            o <<= 1;
        }
    }
    free(cur_v.a);
    qsort(res, n, sizeof(uint64_t), A334104_cmp);
    free(A334104_cache);
    A334104_cache = res;
    A334104_cnt = n;
}

seq_t a(idx_t n)
{
    static uint64_t L = 0;
    if (n == 0) return (seq_t)0;
    while (A334104_cnt < n) {
        if (L >= ((uint64_t)1 << 60)) return (seq_t)0;
        L = L ? L * 4 : 4096;
        A334104_build(L);
    }
    return (seq_t)A334104_cache[n - 1];
}
