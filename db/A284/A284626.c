/*
 * A284626  Positions of 1 in A284622; complement of A284623.
 * https://oeis.org/A284626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 114, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A284626: positions of 1 in the 0011→0 transform of Thue‑Morse.
   The algorithm scans the Thue‑Morse sequence (t(i)=parity of i),
   replacing every occurrence of 0011 by a single 0, then records the
   positions (1‑based) of the remaining 1’s in the transformed sequence.
   Results are cached so that calls with arbitrary n are answered in
   amortised O(1) time after the initial O(n) generation. */
#include <stdint.h>
#include <stdlib.h>

/* internal helper: parity of 64‑bit integer */
static inline int parity_idx(idx_t x) {
    return __builtin_parityll((unsigned long long)x);
}

/* static cache for the sequence values */
static seq_t *cache      = NULL;   /* array of computed a(n)          */
static size_t cache_len  = 0;       /* number of entries in cache     */
static size_t cache_cap  = 0;       /* allocated capacity (elements)  */

/* state of the on‑the‑fly transform */
static idx_t     x_idx    = 0;      /* next input position in Thue‑Morse  */
static idx_t     y_pos    = 1;      /* next output position (1‑based)     */
static size_t    ones_cnt = 0;      /* number of 1's found so far          */

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for n<1, safe guard */

    size_t target = (size_t)n;          /* n fits 64‑bit, small enough for size_t */
    if (target > cache_len) {
        /* enlarge cache */
        if (target > cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
            while (new_cap < target) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort(); /* memory exhausted */
            cache     = tmp;
            cache_cap = new_cap;
        }
        /* generate until we have reached target 1’s */
        while (ones_cnt < target) {
            /* examine next 4 input bits */
            idx_t b0 = parity_idx(x_idx);
            idx_t b1 = parity_idx(x_idx + 1);
            idx_t b2 = parity_idx(x_idx + 2);
            idx_t b3 = parity_idx(x_idx + 3);

            if (b0 == 0 && b1 == 0 && b2 == 1 && b3 == 1) {
                /* 0011 → 0 : skip 4 bits, output 0 */
                x_idx += 4;
                y_pos += 1;
            } else {
                /* output first bit */
                if (b0 == 1) {
                    cache[ones_cnt] = (seq_t)y_pos;
                    ones_cnt += 1;
                }
                x_idx += 1;
                y_pos += 1;
            }
        }
        cache_len = ones_cnt;
    }

    return cache[n - 1];
}
