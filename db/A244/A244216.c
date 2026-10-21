/*
 * A244216  Numbers k that are at least twice the size the largest Catalan
 *          number less than or equal to k.
 * https://oeis.org/A244216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 2568, u32 n <= 9878, u64 n <= 9878,
 *            u128 n <= 9878, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that the largest Catalan ≤ k is m and k ≥ 2*m */
/* OEIS A244216 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* ---------- static state ---------- */
    static seq_t *cache      = NULL;   /* array of sequence values   */
    static size_t cache_cap   = 0;     /* capacity of the array      */
    static size_t cache_size  = 0;     /* number of stored terms     */

    static idx_t  t           = 1;     /* current Catalan index (C_t) */
    static seq_t  C_cur       = 1;     /* C_t                        */
    static seq_t  C_next      = 2;     /* C_{t+1}                    */
    static seq_t  next_k      = 2;     /* the next k to examine      */

    /* ---------- lazy initialisation ---------- */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache_size = 0;
        t = 1;               /* C_1 = 1                         */
        C_cur = 1;
        C_next = 2;          /* C_2 = 2                         */
        next_k = 2;          /* start from k = 2                */
    }

    /* ---------- generate terms until we have `n` of them ---------- */
    while (cache_size < n) {
        /* advance to the correct Catalan interval containing next_k */
        while (next_k > (C_next - 1)) {
            /* move to next Catalan number */
            ++t;
            C_cur = C_next;
            /* C_{t+1} = C_t * 2*(2*t+1)/(t+2) */
            seq_t num = C_cur * (seq_t)2 * (seq_t)(2 * t + 1);
            seq_t den = (seq_t)(t + 2);
            C_next = num / den;
        }

        /* if next_k satisfies the condition, store it */
        if (next_k >= (C_cur * (seq_t)2)) {
            /* grow cache if needed */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? (cache_cap << 1) : 64;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = next_k;
        }

        /* move to the following integer */
        ++next_k;
    }

    /* ---------- return the requested term ---------- */
    return cache[n - ((idx_t)1)];
}
