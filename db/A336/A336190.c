/*
 * A336190  a(n) is the least k such that A336033(k) = n.
 * https://oeis.org/A336190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   search over k: A336033(k) is computed incrementally from prefix
 *            XORs pref(k) = A336033(1) XOR ... XOR A336033(k), using
 *            A336033(k) = #{ 0 <= j <= k-2 : pref(j) = pref(k-1) }; a(n) =
 *            least k with A336033(k) = n
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 67 terms through n = 8117 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static uint64_t *A336190_cnt = NULL;   /* cnt[v] = #{ j <= k : pref(j) = v } */
static size_t A336190_cap = 0;
static uint64_t *A336190_first = NULL; /* first[v] = least k with A336033(k)=v, 0 if none yet */
static size_t A336190_fcap = 0;
static uint64_t A336190_k = 0;         /* last processed k */
static uint64_t A336190_pref = 0;      /* pref(k) */
static int A336190_init = 0;

static void A336190_grow(uint64_t **arr, size_t *cap, uint64_t v)
{
    if (v >= *cap) {
        size_t nc = *cap ? *cap : 1024;
        while (nc <= v) nc *= 2;
        uint64_t *p = (uint64_t *)realloc(*arr, nc * sizeof(uint64_t));
        if (!p) abort();
        memset(p + *cap, 0, (nc - *cap) * sizeof(uint64_t));
        *arr = p;
        *cap = nc;
    }
}

seq_t a(idx_t n)
{
    if (!A336190_init) {
        A336190_init = 1;
        A336190_k = 0;
        A336190_pref = 0;
        A336190_grow(&A336190_cnt, &A336190_cap, 0);
        A336190_cnt[0] = 1;            /* pref(0) = 0 */
    }
    for (;;) {
        if ((size_t)n < A336190_fcap && A336190_first[n] != 0)
            return (seq_t)A336190_first[n];

        uint64_t k = A336190_k + 1;
        uint64_t target = A336190_pref;            /* pref(k-1) */
        /* count among pref(0..k-2): exclude pref(k-1) itself */
        uint64_t val = A336190_cnt[target] - 1;

        A336190_grow(&A336190_first, &A336190_fcap, val);
        if (A336190_first[val] == 0)
            A336190_first[val] = k;

        A336190_pref = target ^ val;
        A336190_grow(&A336190_cnt, &A336190_cap, A336190_pref);
        A336190_cnt[A336190_pref]++;
        A336190_k = k;
    }
}
