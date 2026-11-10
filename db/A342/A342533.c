/*
 * A342533  a(n) is the number of n-gons in A000940 that display symmetry
 *          (reflection / rotation).
 * https://oeis.org/A342533
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (1/(4n^2)) * Sum_{H != 1} |H| * G(H), summed over the
 *            nontrivial * subgroups H of the dihedral group D_n acting on the
 *            n vertices. G(H) = F(H) - * Sum_{K > H} G(K), where K runs over
 *            the subgroups strictly containing H. * F(H) = 2n * #{n-gons
 *            fixed by H} = #{injective homs rho: H -> D_n wh...
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 13 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

typedef struct { int s; uint64_t r; } A342533_El;     /* x -> s*x + r (mod n) */
typedef struct { seq_t q; uint64_t r; } A342533_V;    /* value q*D + r, 0<=r<D */

static uint64_t A342533_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static A342533_El A342533_mk(int s, uint64_t r) { A342533_El e; e.s = s; e.r = r; return e; }

static A342533_El A342533_comp(A342533_El a, A342533_El b, uint64_t n)
{
    A342533_El e;
    uint64_t br = (a.s == 1) ? b.r : (n - b.r) % n;
    e.s = a.s * b.s;
    e.r = (br + a.r) % n;
    return e;
}

static int A342533_eq(A342533_El a, A342533_El b) { return a.s == b.s && a.r == b.r; }

static uint64_t A342533_order(A342533_El a, uint64_t n)
{
    if (a.s < 0) return 2;
    if (a.r == 0) return 1;
    return n / A342533_gcd(a.r, n);
}

static uint64_t A342533_fix(A342533_El a, uint64_t n)
{
    if (a.s > 0) return a.r == 0 ? n : 0;
    if (n & 1) return 1;
    return (a.r & 1) ? 0 : 2;
}

static A342533_V A342533_one(void) { A342533_V v; v.q = 0; v.r = 1; return v; }

static void A342533_mulc(A342533_V *v, uint64_t c, uint64_t D)
{
    unsigned __int128 t = (unsigned __int128)v->r * c;
    v->q = v->q * (seq_t)c + (seq_t)(uint64_t)(t / D);
    v->r = (uint64_t)(t % D);
}

static void A342533_pow(A342533_V *v, uint64_t c, uint64_t e, uint64_t D)
{
    uint64_t i;
    for (i = 0; i < e; i++) A342533_mulc(v, c, D);
}

static void A342533_fact(A342533_V *v, uint64_t k, uint64_t D)
{
    uint64_t i;
    for (i = 2; i <= k; i++) A342533_mulc(v, i, D);
}

/* number of valid injective homs rho: H -> D_n */
static uint64_t A342533_numrho(uint64_t n, uint64_t d, int dih, uint64_t c)
{
    uint64_t m = n / d, cnt = 0, idx, j, i;
    for (idx = 0; idx < 2 * n; idx++) {
        A342533_El T = (idx < n) ? A342533_mk(1, idx) : A342533_mk(-1, idx - n);
        if (A342533_order(T, n) != d) continue;
        if (!dih) {
            A342533_El P = T;
            int ok = 1;
            for (i = 1; i < d; i++) {
                if (A342533_fix(P, n) != 0) { ok = 0; break; }
                P = A342533_comp(P, T, n);
            }
            cnt += ok;
        } else {
            A342533_El Tinv = (T.s == 1) ? A342533_mk(1, (n - T.r) % n) : T;
            uint64_t nu = n + ((n % 2 == 0) ? 1 : 0);
            for (j = 0; j < nu; j++) {
                A342533_El U = (j < n) ? A342533_mk(-1, j) : A342533_mk(1, n / 2);
                A342533_El P;
                int ok = 1;
                if (!A342533_eq(A342533_comp(A342533_comp(U, T, n), U, n), Tinv)) continue;
                P = A342533_mk(1, 0);
                for (i = 0; i < d; i++) {
                    A342533_El Rf;
                    if (A342533_eq(P, U)) { ok = 0; break; }
                    if (A342533_fix(P, n) != (i == 0 ? n : 0)) { ok = 0; break; }
                    Rf = A342533_comp(P, U, n);
                    if (A342533_fix(Rf, n) != A342533_fix(A342533_mk(-1, (c + i * m) % n), n)) { ok = 0; break; }
                    P = A342533_comp(P, T, n);
                }
                cnt += ok;
            }
        }
    }
    return cnt;
}

static A342533_V A342533_F(uint64_t n, uint64_t D, uint64_t d, int dih, uint64_t c)
{
    A342533_V v = A342533_one();
    uint64_t m = n / d;
    A342533_mulc(&v, A342533_numrho(n, d, dih, c), D);
    if (!dih) {
        A342533_pow(&v, d, m, D);
        A342533_fact(&v, m, D);
    } else {
        uint64_t R = 0, m0 = 0, m1 = 0, a, f = (d % 2 == 0) ? 2 : 1;
        for (a = 0; a < m; a++) {
            uint64_t b = (c % m + m - a) % m;
            if (a < b) R++;
            else if (a == b) {
                int key = 0;
                if (d % 2 == 0) {
                    uint64_t r0 = (2 * a) % n;
                    uint64_t diff = (r0 + n - c) % n;
                    key = (int)((diff / m) & 1);
                }
                if (key) m1++; else m0++;
            }
        }
        A342533_pow(&v, 2 * d, R, D);
        A342533_fact(&v, R, D);
        A342533_pow(&v, f, m0, D);
        A342533_fact(&v, m0, D);
        A342533_pow(&v, f, m1, D);
        A342533_fact(&v, m1, D);
    }
    return v;
}

static int A342533_contains(uint64_t n, uint64_t dK, int tK, uint64_t cK,
                            uint64_t dH, int tH, uint64_t cH)
{
    if (dK % dH != 0) return 0;
    if (!tH) return 1;
    if (!tK) return 0;
    return (cH % (n / dK)) == cK;
}

seq_t a(idx_t nn)
{
    uint64_t n = (uint64_t)nn, D = 4 * n * n, d, S = 0, k, i, j;
    uint64_t *sd, *sc, *Fr, *Gr, *sz;
    int *st;
    size_t *ord;
    seq_t *Fq, *Gq;
    A342533_V tot;
    seq_t res;

    for (d = 1; d <= n; d++)
        if (n % d == 0) S += (d > 1 ? 1 : 0) + n / d;

    sd = (uint64_t *)malloc(S * sizeof(uint64_t));
    sc = (uint64_t *)malloc(S * sizeof(uint64_t));
    sz = (uint64_t *)malloc(S * sizeof(uint64_t));
    Fr = (uint64_t *)malloc(S * sizeof(uint64_t));
    Gr = (uint64_t *)malloc(S * sizeof(uint64_t));
    st = (int *)malloc(S * sizeof(int));
    ord = (size_t *)malloc(S * sizeof(size_t));
    Fq = mem_new(seq_t, S);
    Gq = mem_new(seq_t, S);

    k = 0;
    for (d = 1; d <= n; d++) {
        uint64_t m, c;
        A342533_V f0, f1;
        if (n % d != 0) continue;
        m = n / d;
        if (d > 1) {
            A342533_V fc = A342533_F(n, D, d, 0, 0);
            sd[k] = d; st[k] = 0; sc[k] = 0; sz[k] = d;
            Fq[k] = fc.q; Fr[k] = fc.r; k++;
        }
        f0 = A342533_F(n, D, d, 1, 0);
        f1 = f0;
        if (m % 2 == 0) f1 = A342533_F(n, D, d, 1, 1);
        for (c = 0; c < m; c++) {
            A342533_V *fp = (m % 2 == 0 && (c & 1)) ? &f1 : &f0;
            sd[k] = d; st[k] = 1; sc[k] = c; sz[k] = 2 * d;
            Fq[k] = fp->q; Fr[k] = fp->r; k++;
        }
    }

    for (i = 0; i < S; i++) ord[i] = (size_t)i;
    for (i = 1; i < S; i++) {
        size_t x = ord[i];
        j = i;
        while (j > 0 && sz[ord[j - 1]] < sz[x]) { ord[j] = ord[j - 1]; j--; }
        ord[j] = x;
    }

    tot.q = 0; tot.r = 0;
    for (i = 0; i < S; i++) {
        size_t h = ord[i];
        A342533_V g, t;
        g.q = Fq[h]; g.r = Fr[h];
        for (j = 0; j < i; j++) {
            size_t K = ord[j];
            if (sz[K] <= sz[h]) continue;
            if (!A342533_contains(n, sd[K], st[K], sc[K], sd[h], st[h], sc[h])) continue;
            if (g.r >= Gr[K]) { g.r -= Gr[K]; g.q = g.q - Gq[K]; }
            else { g.r = g.r + D - Gr[K]; g.q = g.q - (Gq[K] + 1); }
        }
        Gq[h] = g.q; Gr[h] = g.r;
        t = g;
        A342533_mulc(&t, sz[h], D);
        tot.q += t.q;
        tot.r += t.r;
        if (tot.r >= D) { tot.r -= D; tot.q += 1; }
    }

    res = tot.q;
    free(sd); free(sc); free(sz); free(Fr); free(Gr); free(st); free(ord);
    mem_free(Fq); mem_free(Gq);
    return res;
}
