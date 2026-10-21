/*
 * A269149  Number of nX5 0..3 arrays with some element plus some
 *          horizontally, antidiagonally or vertically adjacent neighbor
 *          totalling three exactly once.
 * https://oeis.org/A269149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic programming over rows for n×5 arrays with entries 0..3,
   counting exactly one adjacent pair (horizontal, vertical, or antidiagonal)
   whose elements sum to 3. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* EdgeList: simple dynamic array of 16‑bit indices */
typedef struct {
    uint16_t *data;
    size_t len;
    size_t cap;
} EdgeList;

/* Ensure a dynamic array has at least `needed` capacity. */
static inline void
ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem)
{
    if (needed > *cap) {
        size_t newcap = (*cap) ? (*cap) * 2 : 16;
        while (newcap < needed) newcap <<= 1;
        *ptr = realloc(*ptr, newcap * elem);
        *cap = newcap;
    }
}

/* Push a value into an EdgeList. */
static inline void
push_back(EdgeList *el, uint16_t val)
{
    ensure_capacity(el->len + 1, &el->cap, (void **)&el->data, sizeof(uint16_t));
    el->data[el->len++] = val;
}

/* 5-row DP: exactly one adjacency sum‑3. */
seq_t a(idx_t n)
{
    /* All 4^5 = 1024 possible row patterns. */
    const size_t TOTAL_PAT = 1024;
    uint8_t digits[TOTAL_PAT][5];
    uint8_t horiz[TOTAL_PAT];

    /* Decode each pattern and count horizontal sum‑3 pairs. */
    for (size_t pat = 0; pat < TOTAL_PAT; ++pat) {
        size_t v = pat;
        for (int j = 0; j < 5; ++j) {
            digits[pat][j] = v & 3;
            v >>= 2;
        }
        unsigned h = 0;
        for (int j = 0; j < 4; ++j)
            if (digits[pat][j] + digits[pat][j + 1] == 3) ++h;
        horiz[pat] = (uint8_t)h;
    }

    /* Keep only rows with at most one horizontal sum‑3 pair. */
    size_t active = 0;
    uint16_t *rows = (uint16_t *)malloc(TOTAL_PAT * sizeof(uint16_t));
    for (size_t pat = 0; pat < TOTAL_PAT; ++pat) {
        if (horiz[pat] <= 1) {
            rows[active++] = (uint16_t)pat;
        }
    }

    /* Adjacency lists: transitions that keep the total count at 0 (adj0)
       or that add the final 1 (adj1). */
    EdgeList *adj0 = (EdgeList *)calloc(active, sizeof(EdgeList));
    EdgeList *adj1 = (EdgeList *)calloc(active, sizeof(EdgeList));

    for (size_t i = 0; i < active; ++i) {
        uint16_t prev_pat = rows[i];
        for (size_t j = 0; j < active; ++j) {
            uint16_t curr_pat = rows[j];
            unsigned vertical = 0, anti = 0;
            for (int k = 0; k < 5; ++k)
                if (digits[prev_pat][k] + digits[curr_pat][k] == 3) ++vertical;
            for (int k = 0; k < 4; ++k)
                if (digits[prev_pat][k] + digits[curr_pat][k + 1] == 3) ++anti;
            unsigned t = vertical + anti + horiz[curr_pat];
            if (t == 0)
                push_back(&adj0[i], (uint16_t)j);
            else if (t == 1)
                push_back(&adj1[i], (uint16_t)j);
        }
    }

    /* DP arrays for counts with current total 0 or 1. */
    seq_t *dp0 = (seq_t *)calloc(active, sizeof(seq_t));
    seq_t *dp1 = (seq_t *)calloc(active, sizeof(seq_t));
    seq_t *new0 = (seq_t *)calloc(active, sizeof(seq_t));
    seq_t *new1 = (seq_t *)calloc(active, sizeof(seq_t));

    /* Initialise for the first row. */
    for (size_t i = 0; i < active; ++i) {
        if (horiz[rows[i]] == 0) dp0[i] = (seq_t)1;
        else if (horiz[rows[i]] == 1) dp1[i] = (seq_t)1;
    }

    if (n == 1) {
        seq_t ans = 0;
        for (size_t i = 0; i < active; ++i) ans += dp1[i];
        /* Free all allocated memory. */
        for (size_t i = 0; i < active; ++i) {
            free(adj0[i].data);
            free(adj1[i].data);
        }
        free(adj0);
        free(adj1);
        free(rows);
        free(dp0);
        free(dp1);
        free(new0);
        free(new1);
        return ans;
    }

    /* Process rows 2..n. */
    for (idx_t row = 2; row <= n; ++row) {
        /* Reset new arrays. */
        for (size_t i = 0; i < active; ++i) new0[i] = new1[i] = (seq_t)0;

        for (size_t prev = 0; prev < active; ++prev) {
            seq_t val0 = dp0[prev];
            seq_t val1 = dp1[prev];
            if (!val0 && !val1) continue;

            EdgeList *e0 = &adj0[prev];
            EdgeList *e1 = &adj1[prev];
            if (val0) {
                for (size_t k = 0; k < e0->len; ++k)
                    new0[e0->data[k]] += val0;
                for (size_t k = 0; k < e1->len; ++k)
                    new1[e1->data[k]] += val0;
            }
            if (val1) {
                for (size_t k = 0; k < e0->len; ++k)
                    new1[e0->data[k]] += val1;
            }
        }

        /* Swap dp and new arrays. */
        seq_t *tmp = dp0; dp0 = new0; new0 = tmp;
        tmp = dp1; dp1 = new1; new1 = tmp;
    }

    /* Sum counts with exactly one adjacency sum‑3. */
    seq_t ans = 0;
    for (size_t i = 0; i < active; ++i) ans += dp1[i];

    /* Free all allocated memory. */
    for (size_t i = 0; i < active; ++i) {
        free(adj0[i].data);
        free(adj1[i].data);
    }
    free(adj0);
    free(adj1);
    free(rows);
    free(dp0);
    free(dp1);
    free(new0);
    free(new1);
    return ans;
}
