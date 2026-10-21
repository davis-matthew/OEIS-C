/*
 * A298874  Solution (a(n)) of the system of 3 equations in Comments.
 * https://oeis.org/A298874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 360, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_resize, used_get, used_set
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A298874:  a(n) defined by a(0)=1, b(0)=2, c(0)=3;
   a(k)=least positive integer not appearing yet as a, b or c,
   b(k)=a(k)+b(k‑1), c(k)=a(k)+2·b(k).  a(n) returned for any n≥0. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_resize.h"
#include "../utils/used_get.h"
#include "../utils/used_set.h"

/* --------- Helpers for dynamic bitset of used integers ---------- */

/* --------- Helpers for dynamic array of seq_t ---------- */

/* --------------- Main sequence function ---------------- */

seq_t a(idx_t n) {
    /* static cache shared across calls */
    static bool initialized = false;
    static seq_t *a_cache = NULL;
    static size_t a_cap = 0;
    static idx_t a_len = 0;           /* number of terms stored */

    static seq_t sum_so_far = 0;      /* sum of a(0..k) so far */
    static seq_t b_prev = 2;          /* b(k-1) */

    static uint8_t *used = NULL;      /* bitset for used integers */
    static size_t used_cap = 0;

    if (!initialized) {
        /* initialise for a(0)=1 */
        seq_resize(1, &a_cache, &a_cap);
        a_cache[0] = 1;
        a_len = 1;
        sum_so_far = 1;
        b_prev = 2;                   /* given b(0)=2 */

        /* mark initial a(0), b(0), c(0)=a(0)+2*b(0)=3 as used */
        used_set(1, &used, &used_cap);
        used_set(2, &used, &used_cap);
        used_set(3, &used, &used_cap);

        initialized = true;
    }

    /* compute terms until we reach n */
    idx_t candidate = 1;              /* start scanning from 1 each time,
                                       because any new unused < previous
                                       a may still be free (rare, but safe) */
    while (a_len <= n) {
        /* find next unused integer */
        while (used_get(candidate, used, used_cap)) candidate++;

        seq_t aval = (seq_t)candidate;

        /* store a(k) */
        seq_resize(a_len + 1, &a_cache, &a_cap);
        a_cache[a_len] = aval;
        a_len++;

        /* mark a(k) as used */
        used_set(candidate, &used, &used_cap);

        /* compute b(k) and c(k) */
        b_prev = sum_so_far + aval + 1;          /* b(k) = a(k) + b(k‑1) */
        seq_t c_val = aval + (b_prev << 1);      /* c(k) = a(k) + 2*b(k) */

        /* mark b(k) and c(k) as used (if they fit in idx_t) */
        if (b_prev <= (seq_t)UINT64_MAX)
            used_set((idx_t)b_prev, &used, &used_cap);
        if (c_val <= (seq_t)UINT64_MAX)
            used_set((idx_t)c_val, &used, &used_cap);

        /* update sum for next step */
        sum_so_far += aval;

        /* next candidate starts above the current one */
        candidate++;
    }

    return a_cache[n];
}
