/*
 * A348237  Coordination sequence for the cpq net with respect to a node where
 *          a hexagon and two octagons meet.
 * https://oeis.org/A348237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Recurrence derived from the rational generating function for
 *            A348237: a(0)=1, a(1)=3, a(2)=6, a(3)=9, a(4)=10, a(5)=13,
 *            a(6)=18, a(7)=21, a(8)=22. For n ≥ 9: a(n) = a(n‑1) – a(n‑2) +
 *            a(n‑3) + a(n‑5) – a(n‑6) + a(n‑7) – a(n‑8)
 * Range:     u8 n <= 90, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t len = 0;

    if (n >= len) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) abort();            /* allocation failed */

        for (size_t i = len; i < new_len; ++i) {
            seq_t val;
            if (i == 0) val = (seq_t)1;
            else if (i == 1) val = (seq_t)3;
            else if (i == 2) val = (seq_t)6;
            else if (i == 3) val = (seq_t)9;
            else if (i == 4) val = (seq_t)10;
            else if (i == 5) val = (seq_t)13;
            else if (i == 6) val = (seq_t)18;
            else if (i == 7) val = (seq_t)21;
            else if (i == 8) val = (seq_t)22;
            else {
                seq_t v = new_cache[i-1];
                v -= new_cache[i-2];
                v += new_cache[i-3];
                v += new_cache[i-5];
                v -= new_cache[i-6];
                v += new_cache[i-7];
                v -= new_cache[i-8];
                val = v;
            }
            new_cache[i] = val;
        }

        cache = new_cache;
        len = new_len;
    }

    return cache[n];
}
