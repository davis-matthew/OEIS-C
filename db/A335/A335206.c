/*
 * A335206  a(n) is the total binary weight of all persolus bitstrings of
 *          length n.
 * https://oeis.org/A335206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = total number of 1 bits in all “persolus” bitstrings of
 *            length n. A persolus bitstring has isolated 1’s and every 0 has
 *            at least one adjacent 0 (except for the unique length‑1 string
 *            “0”). The sequence satisfies the following forward recurrence on
 *            the symbolic states: A – string ends with 1...
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* -------------- memoisation data -------------- */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t computed = 0;          /* largest n already computed */
    static seq_t count[3], sum[3];      /* states for the current length */
    static seq_t next_count[3], next_sum[3];

    if (n < 1) return 0;               /* undefined offset, safety */

    if (cache == NULL) {
        cache_size = n + 1;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;
        for (size_t i = 0; i < cache_size; ++i) cache[i] = 0;
    } else if ((size_t)n + 1 > cache_size) {
        size_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;
        /* initialise new entries to 0 */
        for (size_t i = cache_size; i < new_size; ++i) tmp[i] = 0;
        cache = tmp;
        cache_size = new_size;
    }

    /* -------------- compute up to requested n -------------- */
    if (computed == 0) { /* first time, initialise for n = 1 */
        /* state indices: 0 = A, 1 = B, 2 = C */
        count[0] = 1;  /* string “1” */
        sum[0] = 1;
        count[1] = 1;  /* string “0” */
        sum[1] = 0;
        count[2] = 0;
        sum[2] = 0;
        cache[1] = sum[0] + sum[2]; /* a(1) = 1 */
        computed = 1;
    }

    for (idx_t cur = computed + 1; cur <= n; ++cur) {
        /* Reset next state arrays */
        next_count[0] = next_count[1] = next_count[2] = 0;
        next_sum[0] = next_sum[1] = next_sum[2] = 0;

        /* Transition from state A (index 0) */
        if (count[0]) {
            next_count[1] += count[0];          /* A ->0-> B */
            next_sum[1] += sum[0];              /* no new 1 */
        }

        /* Transition from state B (index 1) */
        if (count[1]) {
            next_count[2] += count[1];          /* B ->0-> C */
            next_sum[2] += sum[1];
        }

        /* Transition from state C (index 2) */
        if (count[2]) {
            /* C ->0-> C */
            next_count[2] += count[2];
            next_sum[2] += sum[2];
            /* C ->1-> A */
            next_count[0] += count[2];
            next_sum[0] += sum[2] + count[2];  /* add one per string */
        }

        /* Store result for this length */
        cache[cur] = next_sum[0] + next_sum[2];

        /* Move to next state */
        count[0] = next_count[0];
        count[1] = next_count[1];
        count[2] = next_count[2];
        sum[0] = next_sum[0];
        sum[1] = next_sum[1];
        sum[2] = next_sum[2];
    }

    computed = n;
    return cache[n];
}
