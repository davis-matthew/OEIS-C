/*
 * A367714  G.f. A(x) satisfies A(x) = 1 / ((1 - x) * (1 - x * (1 + x + x^2) *
 *          A(x^3))).
 * https://oeis.org/A367714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=0..n-1} a(floor(k/3)) * a(n-1-k); a(0) = 1
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 58, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n) {
    /* Static cache for memoisation */
    static seq_t *seq = NULL;
    static idx_t seq_len = 0;   /* number of computed terms */
    static size_t capacity = 0; /* allocated size */

    /* initialise cache if needed */
    if (seq_len == 0) {
        capacity = 1;
        seq = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!seq) abort();
#ifdef __cplusplus
        new (seq) seq_t();         /* construct default element */
        seq[0] = (seq_t)1;         /* a(0) */
#else
        seq[0] = (seq_t)1;         /* a(0) */
#endif
        seq_len = 1;
    }

    /* compute terms up to n if not already computed */
    if (n >= seq_len) {
        /* ensure enough capacity */
        size_t new_cap = capacity;
        while (new_cap <= (size_t)n + 1) {
            new_cap = (new_cap == 0) ? 1 : new_cap * 2;
        }
        if (new_cap > capacity) {
#ifdef __cplusplus
            seq_t *new_seq = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!new_seq) abort();
            /* copy existing elements */
            for (idx_t i = 0; i < capacity; ++i)
                new (new_seq + i) seq_t(seq[i]);   /* copy‑construct */
            /* default‑construct new slots */
            for (idx_t i = capacity; i < new_cap; ++i)
                new (new_seq + i) seq_t();
            /* destroy old elements */
            for (idx_t i = 0; i < capacity; ++i)
                seq[i].~seq_t();
            free(seq);
            seq = new_seq;
#else
            seq_t *new_seq = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!new_seq) abort();
            memcpy(new_seq, seq, capacity * sizeof(seq_t));
            free(seq);
            seq = new_seq;
#endif
            capacity = new_cap;
        }

        /* compute missing terms */
        for (idx_t m = seq_len; m <= n; ++m) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k < m; ++k) {
                idx_t left = k / 3;
                idx_t right = m - 1 - k;
                seq_t prod = seq[left] * seq[right];
                sum += prod;
            }
            seq[m] = sum + (seq_t)1;
        }
        seq_len = n + 1;
    }

    return seq[n];
}
