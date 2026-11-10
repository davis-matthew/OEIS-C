/*
 * A387076  Primes in the order in which they appear in A386482.
 * https://oeis.org/A387076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: simulate A386482 (a(1)=1, a(2)=2; then the greatest
 *            unused k < a(n-1) with gcd(k,a(n-1))>1, else the smallest unused
 *            k > a(n-1) with gcd>1) and list the primes in the order they
 *            appear. The greatest/smallest unused multiple of each prime
 *            factor p is found with per-prime hierarchical bitse...
 * Range:     u8 n <= 22, u16 n <= 370, u32 n <= 4310, u64 n <= 4310,
 *            u128 n <= 4310, big any n
 * Validated: standard, pass, 74 terms through n = 4250 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define A387076_B 65536u
#define A387076_NONE UINT64_MAX

typedef struct {
    uint64_t *w[8];
    uint64_t nb[8];
    int nl;
    uint64_t cnt;
} A387076_hb;

static uint64_t *A387076_U = NULL;     /* used bitset */
static uint16_t *A387076_SPF = NULL;   /* spf of odd numbers, 0 = prime/1 */
static uint64_t A387076_CAP = 0;
static uint32_t *A387076_sp = NULL;    /* primes < 65536 */
static size_t A387076_spn = 0;
static A387076_hb **A387076_H = NULL;  /* per small prime structure */
static uint64_t *A387076_pr = NULL;    /* primes in order of appearance */
static size_t A387076_pcnt = 0, A387076_pcap = 0;
static uint64_t A387076_prev = 0;
static uint64_t A387076_pf[16];
static int A387076_pfn = 0;
static bool A387076_init = false;

static inline bool A387076_used(uint64_t x)
{
    return x < A387076_CAP && ((A387076_U[x >> 6] >> (x & 63)) & 1u);
}

static void A387076_sieve(uint64_t lo, uint64_t hi)
{
    for (size_t i = 1; i < A387076_spn; i++) {
        uint64_t q = A387076_sp[i];
        if (q * q >= hi) break;
        uint64_t s = q * q;
        if (s < lo) {
            s = (lo + q - 1) / q * q;
            if (!(s & 1)) s += q;
        }
        for (uint64_t m = s; m < hi; m += 2 * q)
            if (!A387076_SPF[m >> 1]) A387076_SPF[m >> 1] = (uint16_t)q;
    }
}

static void A387076_ensure(uint64_t need)
{
    if (need < A387076_CAP) return;
    uint64_t nc = A387076_CAP + A387076_CAP / 2;
    if (nc < need + 1) nc = need + 1;
    nc = (nc + 127) & ~(uint64_t)127;
    uint64_t *u = (uint64_t *)realloc(A387076_U, (size_t)(nc / 64) * sizeof(uint64_t));
    if (!u) abort();
    memset(u + A387076_CAP / 64, 0, (size_t)((nc - A387076_CAP) / 64) * sizeof(uint64_t));
    A387076_U = u;
    uint16_t *s = (uint16_t *)realloc(A387076_SPF, (size_t)(nc / 2) * sizeof(uint16_t));
    if (!s) abort();
    memset(s + A387076_CAP / 2, 0, (size_t)((nc - A387076_CAP) / 2) * sizeof(uint16_t));
    A387076_SPF = s;
    A387076_sieve(A387076_CAP, nc);
    A387076_CAP = nc;
}

static void A387076_hb_build(A387076_hb *h, uint64_t p, uint64_t cnt)
{
    for (int l = 0; l < h->nl; l++) free(h->w[l]);
    cnt = (cnt + 63) & ~(uint64_t)63;
    h->cnt = cnt;
    int nl = 0;
    uint64_t nbits = cnt;
    for (;;) {
        uint64_t words = (nbits + 63) / 64;
        h->nb[nl] = nbits;
        h->w[nl] = (uint64_t *)calloc((size_t)words, sizeof(uint64_t));
        if (!h->w[nl]) abort();
        nl++;
        if (words <= 1) break;
        nbits = words;
    }
    h->nl = nl;
    uint64_t *w0 = h->w[0];
    uint64_t x = p;
    for (uint64_t j = 1; j < cnt; j++, x += p)
        if (!A387076_used(x)) w0[j >> 6] |= (uint64_t)1 << (j & 63);
    for (int l = 0; l + 1 < nl; l++) {
        uint64_t words = (h->nb[l] + 63) / 64;
        for (uint64_t w = 0; w < words; w++)
            if (h->w[l][w]) h->w[l + 1][w >> 6] |= (uint64_t)1 << (w & 63);
    }
}

static void A387076_hb_clear(A387076_hb *h, uint64_t j)
{
    for (int l = 0; l < h->nl; l++) {
        uint64_t w = j >> 6;
        h->w[l][w] &= ~((uint64_t)1 << (j & 63));
        if (h->w[l][w]) break;
        j = w;
    }
}

static uint64_t A387076_hb_pred(const A387076_hb *h, uint64_t x)
{
    int l = 0;
    for (;;) {
        uint64_t w = x >> 6;
        unsigned b = (unsigned)(x & 63);
        uint64_t mask = (b == 63) ? ~(uint64_t)0 : (((uint64_t)1 << (b + 1)) - 1);
        uint64_t m = h->w[l][w] & mask;
        if (m) {
            x = (w << 6) | (uint64_t)(63 - __builtin_clzll(m));
            while (l > 0) {
                l--;
                x = (x << 6) | (uint64_t)(63 - __builtin_clzll(h->w[l][x]));
            }
            return x;
        }
        if (w == 0 || l + 1 >= h->nl) return A387076_NONE;
        x = w - 1;
        l++;
    }
}

static uint64_t A387076_hb_succ(const A387076_hb *h, uint64_t x)
{
    if (x >= h->nb[0]) return A387076_NONE;
    int l = 0;
    for (;;) {
        uint64_t w = x >> 6;
        unsigned b = (unsigned)(x & 63);
        uint64_t m = h->w[l][w] & (~(uint64_t)0 << b);
        if (m) {
            x = (w << 6) | (uint64_t)__builtin_ctzll(m);
            while (l > 0) {
                l--;
                x = (x << 6) | (uint64_t)__builtin_ctzll(h->w[l][x]);
            }
            return x;
        }
        x = w + 1;
        l++;
        if (l >= h->nl || x >= h->nb[l]) return A387076_NONE;
    }
}

static A387076_hb *A387076_get(uint64_t p, uint64_t need)
{
    A387076_hb *h = A387076_H[p];
    if (!h) {
        h = (A387076_hb *)calloc(1, sizeof(A387076_hb));
        if (!h) abort();
        A387076_H[p] = h;
        A387076_hb_build(h, p, need + 64 > 1024 ? need + 64 : 1024);
    } else if (need >= h->cnt) {
        uint64_t c = 2 * h->cnt;
        if (c < need + 64) c = need + 64;
        A387076_hb_build(h, p, c);
    }
    return h;
}

static int A387076_factor(uint64_t x, uint64_t *f)
{
    int c = 0;
    if (x > 1 && !(x & 1)) {
        f[c++] = 2;
        while (!(x & 1)) x >>= 1;
    }
    while (x > 1) {
        uint16_t s = A387076_SPF[x >> 1];
        uint64_t q = s ? (uint64_t)s : x;
        f[c++] = q;
        while (x % q == 0) x /= q;
    }
    return c;
}

static void A387076_push(uint64_t v)
{
    if (A387076_pcnt == A387076_pcap) {
        A387076_pcap = A387076_pcap ? A387076_pcap * 2 : 1024;
        uint64_t *t = (uint64_t *)realloc(A387076_pr, A387076_pcap * sizeof(uint64_t));
        if (!t) abort();
        A387076_pr = t;
    }
    A387076_pr[A387076_pcnt++] = v;
}

static void A387076_setup(void)
{
    uint8_t *comp = (uint8_t *)calloc(A387076_B, 1);
    if (!comp) abort();
    A387076_sp = (uint32_t *)malloc(7000 * sizeof(uint32_t));
    if (!A387076_sp) abort();
    for (uint32_t i = 2; i < A387076_B; i++) {
        if (!comp[i]) {
            A387076_sp[A387076_spn++] = i;
            for (uint64_t j = (uint64_t)i * i; j < A387076_B; j += i) comp[j] = 1;
        }
    }
    free(comp);
    A387076_H = (A387076_hb **)calloc(A387076_B + 1, sizeof(A387076_hb *));
    if (!A387076_H) abort();
    A387076_ensure((uint64_t)1 << 16);
    A387076_U[0] |= (uint64_t)1 << 1;   /* 1 used */
    A387076_U[0] |= (uint64_t)1 << 2;   /* 2 used */
    A387076_prev = 2;
    A387076_pfn = A387076_factor(2, A387076_pf);
    A387076_push(2);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (!A387076_init) {
        A387076_init = true;
        A387076_setup();
    }
    while (A387076_pcnt < (size_t)n) {
        uint64_t prev = A387076_prev;
        uint64_t best = 0;
        for (int i = 0; i < A387076_pfn; i++) {
            uint64_t p = A387076_pf[i];
            if (p <= A387076_B) {
                uint64_t j0 = (prev - 1) / p;
                A387076_hb *h = A387076_get(p, j0);
                uint64_t r = A387076_hb_pred(h, j0);
                if (r != A387076_NONE && r > 0 && r * p > best) best = r * p;
            } else {
                uint64_t hi = (prev - 1) / p;
                uint64_t lo = best / p + 1;
                for (uint64_t j = hi; j >= lo; j--) {
                    if (!A387076_used(j * p)) { best = j * p; break; }
                }
            }
        }
        if (best == 0) {
            uint64_t mn = 0;
            for (int i = 0; i < A387076_pfn; i++) {
                uint64_t p = A387076_pf[i];
                uint64_t j0 = prev / p + 1;
                if (p <= A387076_B) {
                    A387076_hb *h = A387076_get(p, j0);
                    uint64_t r;
                    for (;;) {
                        r = A387076_hb_succ(h, j0);
                        if (r != A387076_NONE) break;
                        h = A387076_get(p, 2 * h->cnt);
                    }
                    uint64_t k = r * p;
                    if (mn == 0 || k < mn) mn = k;
                } else {
                    for (uint64_t j = j0;; j++) {
                        uint64_t x = j * p;
                        if (mn && x >= mn) break;
                        if (!A387076_used(x)) { mn = x; break; }
                    }
                }
            }
            best = mn;
        }
        A387076_ensure(best);
        A387076_U[best >> 6] |= (uint64_t)1 << (best & 63);
        A387076_pfn = A387076_factor(best, A387076_pf);
        for (int i = 0; i < A387076_pfn; i++) {
            uint64_t p = A387076_pf[i];
            if (p <= A387076_B && A387076_H[p]) {
                uint64_t j = best / p;
                if (j < A387076_H[p]->cnt) A387076_hb_clear(A387076_H[p], j);
            }
        }
        A387076_prev = best;
        if (best == 2 || ((best & 1) && best > 1 && A387076_SPF[best >> 1] == 0))
            A387076_push(best);
    }
    return (seq_t)A387076_pr[(size_t)n - 1];
}
