/*
 * A385486  Where records occur in A385485.
 * https://oeis.org/A385486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th position where A385485 sets a new record;
 *            A385485(a(n)) = A385487(n). * Found by a search. Every n <= 4096
 *            is checked by brute force. Beyond that, the * candidates are the
 *            structured numbers n = c*(2^m-1), where c is a multiple of * d =
 *            m/gcd(m, 2^m-1) and c <= 2^m. For such n with t...
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 27 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef unsigned __int128 A385486_u128;

typedef struct {
    uint64_t n;
    uint64_t v;
} A385486_entry;

static bool A385486_niven(A385486_u128 P) {
    uint64_t lo = (uint64_t)P;
    uint64_t hi = (uint64_t)(P >> 64);
    unsigned pc = (unsigned)__builtin_popcountll(lo) + (unsigned)__builtin_popcountll(hi);
    if (pc == 0) return false;
    return (P % pc) == 0;
}

/* least j >= jstart with j*n not binary Niven (all j < jstart assumed Niven) */
static uint64_t A385486_val(uint64_t n, uint64_t jstart) {
    uint64_t j = jstart;
    while (A385486_niven((A385486_u128)j * n)) ++j;
    return j;
}

static uint64_t A385486_gcd(uint64_t x, uint64_t y) {
    while (y) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

static int A385486_cmp(const void *p, const void *q) {
    const A385486_entry *x = (const A385486_entry *)p;
    const A385486_entry *y = (const A385486_entry *)q;
    if (x->n < y->n) return -1;
    if (x->n > y->n) return 1;
    return 0;
}

static uint64_t *A385486_rec = NULL;
static size_t A385486_nrec = 0;

static void A385486_push(A385486_entry **arr, size_t *cnt, size_t *cap,
                         uint64_t n, uint64_t v) {
    if (*cnt == *cap) {
        size_t nc = (*cap) ? (*cap) * 2 : 1024;
        *arr = (A385486_entry *)realloc(*arr, nc * sizeof(A385486_entry));
        *cap = nc;
    }
    (*arr)[*cnt].n = n;
    (*arr)[*cnt].v = v;
    ++(*cnt);
}

static void A385486_build(void) {
    A385486_entry *arr = NULL;
    size_t cnt = 0, cap = 0;
    const uint64_t LIMIT = (uint64_t)1 << 62;
    const uint64_t BRUTE = 4096;
    uint64_t n;
    int m;

    for (n = 1; n <= BRUTE; ++n)
        A385486_push(&arr, &cnt, &cap, n, A385486_val(n, 1));

    for (m = 1; m <= 46; ++m) {
        uint64_t M = ((uint64_t)1 << m) - 1;
        uint64_t g = A385486_gcd((uint64_t)m, M % (uint64_t)m);
        uint64_t d = (uint64_t)m / g;
        uint64_t s;
        for (s = 1; s <= 15; s += 2) {
            int b;
            for (b = 0; b <= 40; ++b) {
                uint64_t c = (d * s) << b;
                int a;
                A385486_u128 J;
                if (c > ((uint64_t)1 << m)) break;
                if (c > LIMIT / M) break;
                n = c * M;
                a = __builtin_ctzll(c);
                J = ((((A385486_u128)1) << (m + a)) - 1) / c + 1;
                A385486_push(&arr, &cnt, &cap, n, A385486_val(n, (uint64_t)J));
            }
        }
    }

    qsort(arr, cnt, sizeof(A385486_entry), A385486_cmp);

    {
        size_t i;
        uint64_t best = 0;
        A385486_rec = (uint64_t *)malloc(cnt * sizeof(uint64_t));
        A385486_nrec = 0;
        for (i = 0; i < cnt; ++i) {
            if (arr[i].v > best) {
                best = arr[i].v;
                A385486_rec[A385486_nrec++] = arr[i].n;
            }
        }
    }
    free(arr);
}

seq_t a(idx_t n) {
    if (A385486_rec == NULL) A385486_build();
    if (n < 1 || n > (idx_t)A385486_nrec) return (seq_t)0;
    return (seq_t)A385486_rec[n - 1];
}
