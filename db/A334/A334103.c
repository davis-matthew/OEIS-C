/*
 * A334103  Numbers n for which A329697(n) == 3.
 * https://oeis.org/A334103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search, not a closed form. a(n) is the n-th smallest k with
 *            A329697(k) = 3. Powers of 2 have depth 0. Starting from them,
 *            the code walks the map k -> k - k/P(k) backwards three levels,
 *            where P(k) is the largest prime factor of k: every k = q*p with
 *            p prime, q | m, (p-1)*q = m and no prime factor...
 * Range:     u8 n <= 76, u16 n <= 629, u32 n <= 2821, u64 n <= 2821,
 *            u128 n <= 2821, big any n
 * Validated: standard, pass, 88 terms through n = 2801 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct { uint64_t *v; size_t n, cap; } A334103_vec;

static void A334103_push(A334103_vec *s, uint64_t x)
{
    if (s->n == s->cap) {
        s->cap = s->cap ? s->cap * 2 : 64;
        s->v = (uint64_t *)realloc(s->v, s->cap * sizeof(uint64_t));
    }
    s->v[s->n++] = x;
}

static uint64_t A334103_mulmod(uint64_t a, uint64_t b, uint64_t m)
{
    return (uint64_t)((unsigned __int128)a * b % m);
}

static bool A334103_isprime(uint64_t n)
{
    static const uint64_t sp[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    if (n < 2) return false;
    for (int i = 0; i < 12; i++) {
        if (n == sp[i]) return true;
        if (n % sp[i] == 0) return false;
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (int i = 0; i < 12; i++) {
        uint64_t x = 1, b = sp[i] % n, e = d;
        while (e) {
            if (e & 1) x = A334103_mulmod(x, b, n);
            b = A334103_mulmod(b, b, n);
            e >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (int r = 1; r < s; r++) {
            x = A334103_mulmod(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

typedef struct {
    uint64_t m, limit;
    uint64_t pr[64];
    int ex[64];
    int np;
    A334103_vec *out;
} A334103_ctx;

static void A334103_gen(A334103_ctx *c, int i, uint64_t d)
{
    if (i == c->np) {
        uint64_t p = d + 1;
        if (!A334103_isprime(p)) return;
        uint64_t q = c->m / d;
        for (int j = 0; j < c->np; j++)
            if (c->pr[j] > p && q % c->pr[j] == 0) return;
        unsigned __int128 k = (unsigned __int128)q * p;
        if (k > c->limit) return;
        uint64_t kk = (uint64_t)k;
        if ((kk & (kk - 1)) == 0) return;   /* powers of 2 have depth 0 */
        A334103_push(c->out, kk);
        return;
    }
    uint64_t t = d;
    for (int e = 0; e <= c->ex[i]; e++) {
        A334103_gen(c, i + 1, t);
        if (e < c->ex[i]) t *= c->pr[i];
    }
}

static void A334103_preds(uint64_t m, uint64_t limit, A334103_vec *out)
{
    A334103_ctx c;
    c.m = m; c.limit = limit; c.np = 0; c.out = out;
    uint64_t r = m;
    if ((r & 1) == 0) {
        int e = 0;
        while ((r & 1) == 0) { r >>= 1; e++; }
        c.pr[c.np] = 2; c.ex[c.np++] = e;
    }
    for (uint64_t f = 3; f <= r / f; f += 2) {
        if (r % f == 0) {
            int e = 0;
            while (r % f == 0) { r /= f; e++; }
            c.pr[c.np] = f; c.ex[c.np++] = e;
        }
    }
    if (r > 1) { c.pr[c.np] = r; c.ex[c.np++] = 1; }
    A334103_gen(&c, 0, 1);
}

static int A334103_cmp(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a, y = *(const uint64_t *)b;
    return x < y ? -1 : (x > y);
}

seq_t a(idx_t n)
{
    static uint64_t *terms = 0;
    static size_t cnt = 0;
    static bool done = false;
    if (!done) {
        const uint64_t limit = (uint64_t)1 << 32;
        A334103_vec cur = {0, 0, 0};
        for (int j = 0; j <= 32; j++) A334103_push(&cur, (uint64_t)1 << j);
        for (int lvl = 0; lvl < 3; lvl++) {
            A334103_vec nxt = {0, 0, 0};
            for (size_t i = 0; i < cur.n; i++)
                A334103_preds(cur.v[i], limit, &nxt);
            free(cur.v);
            cur = nxt;
        }
        if (cur.n > 0)
            qsort(cur.v, cur.n, sizeof(uint64_t), A334103_cmp);
        terms = cur.v;
        cnt = cur.n;
        done = true;
    }
    if (n < 1 || (size_t)n > cnt) return (seq_t)0;
    return (seq_t)terms[n - 1];
}
