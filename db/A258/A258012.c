/*
 * A258012  Capped binary boundary codes for fusenes (all orientations and
 *          rotations included).
 * https://oeis.org/A258012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=1; for n>=1, a(n) is the n-th smallest number 2^L + r,
 *            where r is an L-bit rotation of a cyclic 0/1 word that reduces
 *            to 111111 under the cyclic rewrites 011110->11, 01110->101
 *            (A254109) and 0110->1001 (A258009). Computed by generating the
 *            closure from 111111 with the inverse rewrites, group...
 * Range:     u8 n <= 1, u16 n <= 38, u32 n <= 20648, u64 n <= 20648,
 *            u128 n <= 20648, big any n
 * Validated: standard, pass, 55 terms through n = 9461 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint64_t *w;
    size_t n, cap;
    uint64_t *tab;
    size_t tcap;
} A258012_set;

static uint64_t *A258012_levels[64];
static size_t A258012_levcnt[64];
static uint64_t *A258012_terms = 0;
static size_t A258012_nterms = 0, A258012_termcap = 0;
static int A258012_nextL = 6;

static uint64_t A258012_rot(uint64_t w, int L, int k) {
    uint64_t mask = (L >= 64) ? ~(uint64_t)0 : (((uint64_t)1 << L) - 1);
    if (k == 0) return w;
    return ((w << k) | (w >> (L - k))) & mask;
}

static uint64_t A258012_canon(uint64_t w, int L) {
    uint64_t best = w;
    int k;
    for (k = 1; k < L; k++) {
        uint64_t r = A258012_rot(w, L, k);
        if (r < best) best = r;
    }
    return best;
}

static void A258012_tab_grow(A258012_set *s) {
    size_t ncap = s->tcap ? s->tcap * 2 : 1024;
    uint64_t *nt = (uint64_t *)calloc(ncap, sizeof(uint64_t));
    size_t i;
    for (i = 0; i < s->tcap; i++) {
        uint64_t v = s->tab[i];
        if (v) {
            size_t h = (size_t)((v * 0x9E3779B97F4A7C15ULL) >> 20) & (ncap - 1);
            while (nt[h]) h = (h + 1) & (ncap - 1);
            nt[h] = v;
        }
    }
    free(s->tab);
    s->tab = nt;
    s->tcap = ncap;
}

static void A258012_insert(A258012_set *s, uint64_t w, int L) {
    uint64_t c = A258012_canon(w, L);
    size_t h;
    if ((s->n + 1) * 2 > s->tcap) A258012_tab_grow(s);
    h = (size_t)((c * 0x9E3779B97F4A7C15ULL) >> 20) & (s->tcap - 1);
    while (s->tab[h]) {
        if (s->tab[h] == c) return;
        h = (h + 1) & (s->tcap - 1);
    }
    s->tab[h] = c;
    if (s->n == s->cap) {
        s->cap = s->cap ? s->cap * 2 : 256;
        s->w = (uint64_t *)realloc(s->w, s->cap * sizeof(uint64_t));
    }
    s->w[s->n++] = c;
}

static void A258012_expand(A258012_set *s, int L, int srcL, uint64_t pat, int m,
                           uint64_t rep) {
    size_t j;
    uint64_t *src;
    if (srcL < 6 || srcL >= 64 || !A258012_levels[srcL]) return;
    src = A258012_levels[srcL];
    for (j = 0; j < A258012_levcnt[srcL]; j++) {
        int i;
        for (i = 0; i < srcL; i++) {
            uint64_t r = A258012_rot(src[j], srcL, i);
            if ((r >> (srcL - m)) == pat) {
                uint64_t nw = (rep << (srcL - m)) |
                              (r & (((uint64_t)1 << (srcL - m)) - 1));
                A258012_insert(s, nw, L);
            }
        }
    }
}

static int A258012_cmp(const void *a, const void *b) {
    uint64_t x = *(const uint64_t *)a, y = *(const uint64_t *)b;
    return (x < y) ? -1 : (x > y);
}

static void A258012_gen(int L) {
    A258012_set s;
    size_t idx;
    uint64_t *batch;
    size_t bn = 0, bcap = 0;
    memset(&s, 0, sizeof(s));
    if (L == 6) {
        A258012_insert(&s, 63, 6);
    } else {
        A258012_expand(&s, L, L - 4, 3, 2, 30);   /* 11 -> 011110 */
        A258012_expand(&s, L, L - 2, 5, 3, 14);   /* 101 -> 01110 */
    }
    /* length-preserving closure: 1001 -> 0110 */
    for (idx = 0; idx < s.n; idx++) {
        uint64_t w = s.w[idx];
        int i;
        for (i = 0; i < L; i++) {
            uint64_t r = A258012_rot(w, L, i);
            if ((r >> (L - 4)) == 9) {
                uint64_t nw = ((uint64_t)6 << (L - 4)) |
                              (r & (((uint64_t)1 << (L - 4)) - 1));
                A258012_insert(&s, nw, L);
            }
        }
    }
    /* collect all distinct rotations with the cap bit */
    batch = 0;
    for (idx = 0; idx < s.n; idx++) {
        uint64_t w = s.w[idx];
        int p = L, k;
        for (k = 1; k < L; k++) {
            if (A258012_rot(w, L, k) == w) { p = k; break; }
        }
        for (k = 0; k < p; k++) {
            if (bn == bcap) {
                bcap = bcap ? bcap * 2 : 256;
                batch = (uint64_t *)realloc(batch, bcap * sizeof(uint64_t));
            }
            batch[bn++] = ((uint64_t)1 << L) | A258012_rot(w, L, k);
        }
    }
    if (bn) qsort(batch, bn, sizeof(uint64_t), A258012_cmp);
    if (A258012_nterms + bn > A258012_termcap) {
        A258012_termcap = (A258012_nterms + bn) * 2 + 16;
        A258012_terms = (uint64_t *)realloc(A258012_terms,
                                            A258012_termcap * sizeof(uint64_t));
    }
    for (idx = 0; idx < bn; idx++) A258012_terms[A258012_nterms++] = batch[idx];
    free(batch);
    free(s.tab);
    A258012_levels[L] = s.w;
    A258012_levcnt[L] = s.n;
}

seq_t a(idx_t n) {
    if (A258012_nterms == 0) {
        A258012_termcap = 64;
        A258012_terms = (uint64_t *)malloc(A258012_termcap * sizeof(uint64_t));
        A258012_terms[0] = 1;
        A258012_nterms = 1;
    }
    while ((idx_t)A258012_nterms <= n && A258012_nextL <= 60) {
        A258012_gen(A258012_nextL);
        A258012_nextL += 2;
    }
    if ((idx_t)A258012_nterms <= n) return (seq_t)0;
    return (seq_t)A258012_terms[(size_t)n];
}
