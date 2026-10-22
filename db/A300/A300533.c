/*
 * A300533  Number of nX2 0..1 arrays with every element equal to 0, 2, 3 or 4
 *          horizontally, vertically or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A300533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 45, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 4*a(n-2) + 2*a(n-3) – a(n-6)
   with initial values a(1)=1, a(2)=3, a(3)=5, a(4)=16, a(5)=40, a(6)=113 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *memo = NULL;
    static size_t memo_cap = 0;    /* allocated size minus one (index 0 unused) */
    static idx_t computed_max = 6; /* last index that has been computed */

    /* initialise the cache on first call */
    if (memo == NULL) {
        memo_cap = 16; /* initial allocation (covers indices 0..16) */
        memo = (seq_t *)malloc((memo_cap + 1) * sizeof(seq_t));
        if (!memo) abort();                     /* memory allocation failed */
        /* zero initial entries */
        for (size_t i = 0; i <= memo_cap; ++i) memo[i] = 0;
        /* pre‑populate the known first six values */
        memo[1] = (seq_t)1;
        memo[2] = (seq_t)3;
        memo[3] = (seq_t)5;
        memo[4] = (seq_t)16;
        memo[5] = (seq_t)40;
        memo[6] = (seq_t)113;
    }

    /* extend the allocation if necessary */
    if ((size_t)n > memo_cap) {
        size_t new_cap = memo_cap ? memo_cap * 2 : 16;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(memo, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) abort();             /* realloc failed */
        memo = tmp;
        memo_cap = new_cap;
        /* new slots are left uninitialised; they will be filled when computed */
    }

    /* compute missing terms only once */
    if ((size_t)n > (size_t)computed_max) {
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            memo[i] =
                memo[i - 1]
              + (seq_t)4 * memo[i - 2]
              + (seq_t)2 * memo[i - 3]
              - memo[i - 6];
        }
        computed_max = n;
    }

    return memo[n];
}
