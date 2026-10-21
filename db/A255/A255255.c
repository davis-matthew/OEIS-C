/*
 * A255255  Number of 2-colorings of a 4 X n rectangle such that no nontrivial
 *          subsquare has monochromatic corners.
 * https://oeis.org/A255255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of 2‑colorings of a 4×n rectangle with no
 *            monochromatic‑corner subsquare; computed by DP over the last 3
 *            columns
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* 4‑bit columns: values 0..15 */
#define ENCODE(c3, c2, c1) (((uint16_t)(c3) << 8) | ((uint16_t)(c2) << 4) | (uint16_t)(c1))

/* Transition tables: for every state (c1,c2,c3), list all compatible
   new columns c4.  Maximum 16 transitions per state. */
static uint16_t trans_table[4096][16];
static uint8_t  trans_len[4096];

/* Transition precomputation. */
static void init_transitions(void)
{
    for (uint16_t state = 0; state < 4096; ++state) {
        uint16_t c3 = state >> 8;
        uint16_t c2 = (state >> 4) & 0xF;
        uint16_t c1 = state & 0xF;
        uint8_t len = 0;
        for (uint8_t newcol = 0; newcol < 16; ++newcol) {
            bool ok = true;
            /* 2×2 squares between c3 and newcol */
            for (int r = 0; r <= 2; ++r) {
                int b1 = (c3 >> r) & 1;
                int b2 = (newcol >> r) & 1;
                int b3 = (c3 >> (r + 1)) & 1;
                int b4 = (newcol >> (r + 1)) & 1;
                if (b1 == b2 && b1 == b3 && b1 == b4) { ok = false; break; }
            }
            if (!ok) continue;
            /* 3×3 squares between c2 and newcol */
            for (int r = 0; r <= 1; ++r) {
                int b1 = (c2 >> r) & 1;
                int b2 = (newcol >> r) & 1;
                int b3 = (c2 >> (r + 2)) & 1;
                int b4 = (newcol >> (r + 2)) & 1;
                if (b1 == b2 && b1 == b3 && b1 == b4) { ok = false; break; }
            }
            if (!ok) continue;
            /* 4×4 square between c1 and newcol */
            int b1 = (c1 >> 0) & 1;
            int b2 = (newcol >> 0) & 1;
            int b3 = (c1 >> 3) & 1;
            int b4 = (newcol >> 3) & 1;
            if (b1 == b2 && b1 == b3 && b1 == b4) continue;
            trans_table[state][len++] = ENCODE(newcol, c3, c2);
        }
        trans_len[state] = len;
    }
}

/* Cache for a(n). */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* DP arrays for last 3 columns. */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;

/* Ensure cache has room for at least n. */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap) return;
    size_t newcap = cache_cap ? cache_cap << 1 : 64;
    while (newcap <= n) newcap <<= 1;
    seq_t *newc;
#ifdef __cplusplus
    newc = new seq_t[newcap];
#else
    newc = (seq_t *)malloc(newcap * sizeof(seq_t));
#endif
    if (!newc) exit(1);
    for (size_t i = 0; i < cache_cap; ++i) newc[i] = cache[i];
    for (size_t i = cache_cap; i < newcap; ++i) newc[i] = 0;
    if (cache) {
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
    }
    cache = newc;
    cache_cap = newcap;
}

/* Allocate DP arrays once. */
static void init_dp_arrays(void)
{
    if (!dp_cur) {
        dp_cur  = (seq_t *)malloc(4096 * sizeof(seq_t));
        dp_next = (seq_t *)malloc(4096 * sizeof(seq_t));
        for (int i = 0; i < 4096; ++i) {
            dp_cur[i]  = 0;
            dp_next[i] = 0;
        }
    }
}

/* Compute a(3) and initialise dp_cur. */
static void init_dp_and_cache(void)
{
    /* 0 → 1, 1 → 16, 2 → 178 are known. */
    cache[0] = 1;
    cache[1] = 16;
    cache[2] = 178;

    /* Enumerate all triples (c1, c2, c3) and check constraints. */
    seq_t total3 = 0;
    for (uint16_t c1 = 0; c1 < 16; ++c1) {
        for (uint16_t c2 = 0; c2 < 16; ++c2) {
            /* 2×2 between c1 and c2 */
            bool ok12 = true;
            for (int r = 0; r <= 2; ++r) {
                int b1 = (c1 >> r) & 1;
                int b2 = (c2 >> r) & 1;
                int b3 = (c1 >> (r + 1)) & 1;
                int b4 = (c2 >> (r + 1)) & 1;
                if (b1 == b2 && b1 == b3 && b1 == b4) { ok12 = false; break; }
            }
            if (!ok12) continue;
            for (uint16_t c3 = 0; c3 < 16; ++c3) {
                /* 2×2 between c2 and c3 */
                bool ok23 = true;
                for (int r = 0; r <= 2; ++r) {
                    int b1 = (c2 >> r) & 1;
                    int b2 = (c3 >> r) & 1;
                    int b3 = (c2 >> (r + 1)) & 1;
                    int b4 = (c3 >> (r + 1)) & 1;
                    if (b1 == b2 && b1 == b3 && b1 == b4) { ok23 = false; break; }
                }
                if (!ok23) continue;
                /* 3×3 between c1 and c3 */
                bool ok13 = true;
                for (int r = 0; r <= 1; ++r) {
                    int b1 = (c1 >> r) & 1;
                    int b2 = (c3 >> r) & 1;
                    int b3 = (c1 >> (r + 2)) & 1;
                    int b4 = (c3 >> (r + 2)) & 1;
                    if (b1 == b2 && b1 == b3 && b1 == b4) { ok13 = false; break; }
                }
                if (!ok13) continue;

                uint16_t state = ENCODE(c3, c2, c1);
                dp_cur[state] = 1;   /* one sequence for this triple */
                ++total3;
            }
        }
    }
    cache[3] = total3;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_transitions();
        init_dp_arrays();
        ensure_cache(n);
        init_dp_and_cache();
        initialized = true;
    } else if (n >= cache_cap) {
        ensure_cache(n);
    }

    if (n <= 3) {
        return cache[n];
    }

    /* Extend cache up to n if needed. */
    for (idx_t cur = 4; cur <= n; ++cur) {
        if (cache[cur] != 0) continue;          /* already computed */

        /* dp_next clears automatically each iteration */
        for (int i = 0; i < 4096; ++i) dp_next[i] = 0;

        for (uint16_t state = 0; state < 4096; ++state) {
            seq_t val = dp_cur[state];
            if (val == 0) continue;
            uint8_t len = trans_len[state];
            for (uint8_t k = 0; k < len; ++k) {
                uint16_t dest = trans_table[state][k];
                dp_next[dest] += val;
            }
        }

        /* Sum counts for a(cur). */
        seq_t sum = 0;
        for (int i = 0; i < 4096; ++i) sum += dp_next[i];
        cache[cur] = sum;

        /* Prepare for next iteration. */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    return cache[n];
}
