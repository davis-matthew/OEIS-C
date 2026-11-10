/*
 * A358470  Number of 3's that appeared by n-th step when constructing
 *          A030717.
 * https://oeis.org/A358470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 3's among the terms of A030717 after n steps
 *            (step 1 = list [1]); computed by simulating the frequency-count
 *            procedure
 * Range:     u8 n <= 51, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    uint64_t *cnt;    /* cnt[v] = number of occurrences of v in the first list */
    size_t cap;
    uint64_t maxv;    /* largest value present */
    uint64_t *res;    /* res[k-1] = number of 3's after step k */
    size_t nres;
    size_t rescap;
} A358470_State;

static void A358470_grow(A358470_State *s, size_t need)
{
    if (need < s->cap) return;
    size_t nc = s->cap;
    while (nc <= need) nc *= 2;
    uint64_t *p = (uint64_t *)malloc(nc * sizeof(uint64_t));
    for (size_t i = 0; i < nc; ++i) p[i] = (i < s->cap) ? s->cnt[i] : 0;
    free(s->cnt);
    s->cnt = p;
    s->cap = nc;
}

seq_t a(idx_t n)
{
    static A358470_State st;
    static bool init = false;
    if (!init) {
        st.cap = 64;
        st.cnt = (uint64_t *)malloc(st.cap * sizeof(uint64_t));
        for (size_t i = 0; i < st.cap; ++i) st.cnt[i] = 0;
        st.cnt[1] = 1;
        st.maxv = 1;
        st.rescap = 128;
        st.res = (uint64_t *)malloc(st.rescap * sizeof(uint64_t));
        st.res[0] = 0;
        st.nres = 1;
        init = true;
    }
    while (st.nres < (size_t)n) {
        /* snapshot frequencies of distinct values in ascending order */
        size_t d = 0;
        uint64_t *fs = (uint64_t *)malloc((size_t)(st.maxv + 1) * sizeof(uint64_t));
        for (uint64_t v = 1; v <= st.maxv; ++v) {
            if (st.cnt[v] > 0) fs[d++] = st.cnt[v];
        }
        for (size_t i = 0; i < d; ++i) {
            uint64_t f = fs[i];
            A358470_grow(&st, (size_t)f + 1);
            st.cnt[f] += 1;
            if (f > st.maxv) st.maxv = f;
        }
        free(fs);
        if (st.nres >= st.rescap) {
            st.rescap *= 2;
            uint64_t *p = (uint64_t *)malloc(st.rescap * sizeof(uint64_t));
            for (size_t i = 0; i < st.nres; ++i) p[i] = st.res[i];
            free(st.res);
            st.res = p;
        }
        A358470_grow(&st, 4);
        st.res[st.nres++] = st.cnt[3];
    }
    return (seq_t)st.res[(size_t)n - 1];
}
