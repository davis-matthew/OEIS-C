/*
 * A333550  Records in A333549.
 * https://oeis.org/A333550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th record value in A333549 (the values A005132(m-1)-m
 *            that are already in A005132 although A005132(m-1) >= m).
 *            Computed by simulating Recaman's sequence and skipping whole
 *            runs of steps that alternate subtract/add. The set of visited
 *            values is stored as a treap of disjoint intervals
 * Range:     u8 n <= 8, u16 n <= 48, u32 n <= 326, u64 n <= 423,
 *            u128 n <= 423, big any n
 * Validated: standard, pass, 59 terms through n = 374 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

typedef struct { uint64_t lo, hi; uint32_t pr; int l, r; } A333550_node;

static A333550_node *A333550_N = NULL;
static int A333550_cnt = 0, A333550_cap = 0, A333550_freeh = -1, A333550_root = -1;
static uint32_t A333550_rng = 2463534242u;

static uint64_t *A333550_rec = NULL;
static idx_t A333550_rlen = 0, A333550_rcap = 0;
static bool A333550_have = false;
static uint64_t A333550_best = 0;
static bool A333550_init = false;
static uint64_t A333550_m = 1, A333550_v = 0;

static uint32_t A333550_rand(void)
{
    uint32_t x = A333550_rng;
    x ^= x << 13; x ^= x >> 17; x ^= x << 5;
    A333550_rng = x;
    return x;
}

static int A333550_new_node(uint64_t lo, uint64_t hi)
{
    int t;
    if (A333550_freeh >= 0) {
        t = A333550_freeh;
        A333550_freeh = A333550_N[t].l;
    } else {
        if (A333550_cnt >= A333550_cap) {
            int nc = A333550_cap ? A333550_cap * 2 : 1024;
            A333550_node *p = (A333550_node *)realloc(A333550_N, (size_t)nc * sizeof(A333550_node));
            if (!p) abort();
            A333550_N = p;
            A333550_cap = nc;
        }
        t = A333550_cnt++;
    }
    A333550_N[t].lo = lo;
    A333550_N[t].hi = hi;
    A333550_N[t].pr = A333550_rand();
    A333550_N[t].l = -1;
    A333550_N[t].r = -1;
    return t;
}

static void A333550_split(int t, uint64_t key, int *a, int *b)
{
    if (t < 0) { *a = -1; *b = -1; return; }
    if (A333550_N[t].lo < key) {
        int ra, rb;
        A333550_split(A333550_N[t].r, key, &ra, &rb);
        A333550_N[t].r = ra;
        *a = t; *b = rb;
    } else {
        int la, lb;
        A333550_split(A333550_N[t].l, key, &la, &lb);
        A333550_N[t].l = lb;
        *a = la; *b = t;
    }
}

static int A333550_merge(int a, int b)
{
    if (a < 0) return b;
    if (b < 0) return a;
    if (A333550_N[a].pr > A333550_N[b].pr) {
        int r = A333550_merge(A333550_N[a].r, b);
        A333550_N[a].r = r;
        return a;
    } else {
        int l = A333550_merge(a, A333550_N[b].l);
        A333550_N[b].l = l;
        return b;
    }
}

static void A333550_insert_node(int x)
{
    int a, b;
    A333550_split(A333550_root, A333550_N[x].lo, &a, &b);
    A333550_root = A333550_merge(A333550_merge(a, x), b);
}

static void A333550_erase(uint64_t key)
{
    int a, b, mid, c;
    A333550_split(A333550_root, key, &a, &b);
    A333550_split(b, key + 1, &mid, &c);
    if (mid >= 0) {
        A333550_N[mid].l = A333550_freeh;
        A333550_freeh = mid;
    }
    A333550_root = A333550_merge(a, c);
}

/* node with largest lo <= y, or -1 */
static int A333550_pred(uint64_t y)
{
    int t = A333550_root, res = -1;
    while (t >= 0) {
        if (A333550_N[t].lo <= y) { res = t; t = A333550_N[t].r; }
        else t = A333550_N[t].l;
    }
    return res;
}

/* node with smallest lo >= y, or -1 */
static int A333550_succ(uint64_t y)
{
    int t = A333550_root, res = -1;
    while (t >= 0) {
        if (A333550_N[t].lo >= y) { res = t; t = A333550_N[t].l; }
        else t = A333550_N[t].r;
    }
    return res;
}

static bool A333550_visited(uint64_t y)
{
    int p = A333550_pred(y);
    return p >= 0 && A333550_N[p].hi >= y;
}

static void A333550_add_interval(uint64_t a, uint64_t b)
{
    int p = A333550_pred(a);
    if (p >= 0 && A333550_N[p].hi + 1 >= a) {
        uint64_t plo = A333550_N[p].lo, phi = A333550_N[p].hi;
        a = plo;
        if (phi > b) b = phi;
        A333550_erase(plo);
    }
    for (;;) {
        int s = A333550_succ(a);
        if (s < 0 || A333550_N[s].lo > b + 1) break;
        {
            uint64_t slo = A333550_N[s].lo, shi = A333550_N[s].hi;
            if (shi > b) b = shi;
            A333550_erase(slo);
        }
    }
    A333550_insert_node(A333550_new_node(a, b));
}

static void A333550_record(uint64_t x)
{
    if (A333550_have && x <= A333550_best) return;
    A333550_have = true;
    A333550_best = x;
    if (A333550_rlen >= A333550_rcap) {
        idx_t nc = A333550_rcap ? A333550_rcap * 2 : 64;
        uint64_t *t = (uint64_t *)realloc(A333550_rec, (size_t)nc * sizeof(uint64_t));
        if (!t) abort();
        A333550_rec = t;
        A333550_rcap = nc;
    }
    A333550_rec[A333550_rlen++] = x;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (!A333550_init) {
        A333550_init = true;
        A333550_add_interval(0, 0);   /* A005132(0) = 0 */
        A333550_m = 1;
        A333550_v = 0;
    }
    while (A333550_rlen < n) {
        uint64_t m = A333550_m, v = A333550_v;
        if (v >= m) {
            uint64_t L = v - m;
            int p = A333550_pred(L);
            uint64_t u = (A333550_N[p].hi >= L) ? L : A333550_N[p].hi;
            uint64_t c1 = L - u;
            if (c1 > 0) {
                uint64_t k = c1;
                bool sub2 = (L >= m + 1);
                uint64_t Tv = 0;
                if (sub2) {
                    uint64_t lim, j = 0;
                    Tv = L - m - 1;
                    lim = Tv / 3;
                    if (lim > c1 - 1) lim = c1 - 1;
                    while (j <= lim) {
                        uint64_t y = Tv - 3 * j;
                        int q = A333550_pred(y);
                        if (q >= 0 && A333550_N[q].hi >= y) {
                            j = (Tv - A333550_N[q].lo) / 3 + 1;
                        } else {
                            k = j;
                            break;
                        }
                    }
                }
                if (k > 0) {
                    if (sub2) A333550_record(Tv);
                    A333550_add_interval(L - k + 1, L);
                    A333550_add_interval(v + 1, v + k);
                    A333550_m = m + 2 * k;
                    A333550_v = v + k;
                    continue;
                }
            }
        }
        /* single generic step */
        {
            uint64_t next;
            if (v >= m) {
                uint64_t t = v - m;
                if (A333550_visited(t)) {
                    A333550_record(t);
                    next = v + m;
                } else {
                    next = t;
                }
            } else {
                next = v + m;
            }
            A333550_add_interval(next, next);
            A333550_v = next;
            A333550_m = m + 1;
        }
    }
    return (seq_t)A333550_rec[n - 1];
}
