/*
 * A256242  Numbers having digits in nondecreasing order and repeatedly
 *          setting n := A066308(n) yields a constant nonzero n.
 * https://oeis.org/A256242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search. For each length L (in increasing order), a nondecreasing
 *            zero-free number is 1^(L-k) D, where D is the sorted list of its
 *            k digits that are >= 2. Since A066308 depends only on the
 *            multiset of digits, x -> (L + e)*p, where p = prod(D) and e =
 *            sum(d-1 for d in D). For every 7-smooth p (p no...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 31, u64 n <= 53, u128 n <= 90,
 *            big any n
 * Validated: standard, pass, 51 terms through n = 143 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define A256242_PMAX 10000000000000ULL   /* 1e13 */
#define A256242_MAXE 512
#define A256242_LMAX 5000

typedef struct { uint64_t p; uint16_t e; uint16_t kmin; } A256242_ent;
typedef struct { uint32_t L; uint8_t k; uint8_t c[8]; } A256242_rec;

static A256242_ent *A256242_E = 0;
static size_t A256242_nE = 0, A256242_capE = 0;
static A256242_rec *A256242_T = 0;
static size_t A256242_nT = 0, A256242_capT = 0;
static uint32_t A256242_Ldone = 0;
static int A256242_inited = 0;
static uint16_t A256242_tmpk[A256242_MAXE + 1];

static void A256242_fact(uint64_t r, int mind, int e, int k)
{
    if (r == 1) {
        if (e <= A256242_MAXE && k < A256242_tmpk[e]) A256242_tmpk[e] = (uint16_t)k;
        return;
    }
    for (int d = mind; d <= 9; ++d)
        if (r % (uint64_t)d == 0) A256242_fact(r / (uint64_t)d, d, e + d - 1, k + 1);
}

static void A256242_push_ent(uint64_t p, int e, int k)
{
    if (A256242_nE == A256242_capE) {
        A256242_capE = A256242_capE ? 2 * A256242_capE : 4096;
        A256242_E = (A256242_ent *)realloc(A256242_E, A256242_capE * sizeof(A256242_ent));
    }
    A256242_E[A256242_nE].p = p;
    A256242_E[A256242_nE].e = (uint16_t)e;
    A256242_E[A256242_nE].kmin = (uint16_t)k;
    A256242_nE++;
}

static void A256242_init(void)
{
    for (uint64_t p2 = 1; p2 <= A256242_PMAX; p2 *= 2) {
        for (uint64_t p3 = p2; p3 <= A256242_PMAX; p3 *= 3) {
            for (uint64_t p5 = p3; p5 <= A256242_PMAX; p5 *= 5) {
                if (p2 > 1 && p5 != p3) break;          /* multiple of 10 */
                for (uint64_t p = p5; p <= A256242_PMAX; p *= 7) {
                    for (int i = 0; i <= A256242_MAXE; ++i) A256242_tmpk[i] = 0xFFFF;
                    A256242_fact(p, 2, 0, 0);
                    for (int i = 0; i <= A256242_MAXE; ++i)
                        if (A256242_tmpk[i] != 0xFFFF) A256242_push_ent(p, i, A256242_tmpk[i]);
                    if (p > A256242_PMAX / 7) break;
                }
                if (p5 > A256242_PMAX / 5) break;
            }
            if (p3 > A256242_PMAX / 3) break;
        }
        if (p2 > A256242_PMAX / 2) break;
    }
    A256242_inited = 1;
}

/* chain from y reaches a nonzero fixed point of A066308 */
static int A256242_good(uint64_t y)
{
    uint64_t t = y;
    while (t) { if (t % 10 == 0) return 0; t /= 10; }
    unsigned __int128 lim = 1;
    for (int i = 0; i < 37; ++i) lim *= 10;
    unsigned __int128 x = y;
    for (int it = 0; it < 1000; ++it) {
        if (x >= lim) return 0;
        unsigned __int128 s = 0, pr = 1, z = x;
        while (z != 0) {
            unsigned d = (unsigned)(z % 10);
            if (d == 0) return 0;
            s += d; pr *= d; z /= 10;
        }
        unsigned __int128 f = s * pr;
        if (f == x) return 1;
        x = f;
    }
    return 0;
}

static uint8_t A256242_cnt[8];

static void A256242_push_rec(uint32_t L, int k)
{
    if (A256242_nT == A256242_capT) {
        A256242_capT = A256242_capT ? 2 * A256242_capT : 256;
        A256242_T = (A256242_rec *)realloc(A256242_T, A256242_capT * sizeof(A256242_rec));
    }
    A256242_T[A256242_nT].L = L;
    A256242_T[A256242_nT].k = (uint8_t)k;
    memcpy(A256242_T[A256242_nT].c, A256242_cnt, 8);
    A256242_nT++;
}

static void A256242_coll(uint64_t r, int mind, int erem, int k, uint32_t L)
{
    if (r == 1) {
        if (erem == 0 && (uint32_t)k <= L) A256242_push_rec(L, k);
        return;
    }
    if ((uint32_t)k + 1 > L) return;
    for (int d = mind; d <= 9; ++d) {
        if (d - 1 > erem) break;
        if (r % (uint64_t)d == 0) {
            A256242_cnt[d - 2]++;
            A256242_coll(r / (uint64_t)d, d, erem - (d - 1), k + 1, L);
            A256242_cnt[d - 2]--;
        }
    }
}

static int A256242_cmp(const void *pa, const void *pb)
{
    const A256242_rec *x = (const A256242_rec *)pa, *y = (const A256242_rec *)pb;
    if (x->k != y->k) return x->k < y->k ? -1 : 1;
    for (int i = 0; i < 8; ++i)
        if (x->c[i] != y->c[i]) return x->c[i] > y->c[i] ? -1 : 1;
    return 0;
}

static void A256242_do_length(uint32_t L)
{
    size_t start = A256242_nT;
    for (size_t i = 0; i < A256242_nE; ++i) {
        const A256242_ent *en = &A256242_E[i];
        if (en->kmin > L) continue;
        uint64_t y = (uint64_t)(L + en->e) * en->p;
        if (y % 10 == 0) continue;
        if (!A256242_good(y)) continue;
        memset(A256242_cnt, 0, sizeof A256242_cnt);
        A256242_coll(en->p, 2, en->e, 0, L);
    }
    if (A256242_nT - start > 1)
        qsort(A256242_T + start, A256242_nT - start, sizeof(A256242_rec), A256242_cmp);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (!A256242_inited) A256242_init();
    while (A256242_nT < n && A256242_Ldone < A256242_LMAX) {
        A256242_Ldone++;
        A256242_do_length(A256242_Ldone);
    }
    if (A256242_nT < n) return (seq_t)0;
    const A256242_rec *r = &A256242_T[n - 1];
    seq_t x = 0;
    for (uint32_t i = 0; i < r->L - r->k; ++i) x = x * 10 + 1;
    for (int d = 2; d <= 9; ++d)
        for (int c = 0; c < r->c[d - 2]; ++c) x = x * 10 + d;
    return x;
}
