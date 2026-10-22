/*
 * A329693  Number of excursions of length n with Motzkin-steps avoiding the
 *          consecutive steps UH, HH, HD and DU.
 * https://oeis.org/A329693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 26, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A329693: number of excursions of length n avoiding the
   consecutive Motzkin steps UH, HH, HD and DU.
   This implementation uses a simple DP that keeps track of the
   current altitude and the last step type.  The algorithm runs in
   O(n^2) time and uses O(n) memory. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* empty excursion */
    size_t N = (size_t)n;

    /* allocate two work arrays of size (N+1)*3 */
    size_t cap = (N + 1) * 3;
    seq_t *cur  = (seq_t*)calloc(cap, sizeof(seq_t));
    seq_t *next = (seq_t*)calloc(cap, sizeof(seq_t));
    if (!cur || !next) abort();    /* allocation failed */

    /* step indices: 0 = U, 1 = H, 2 = D */
    const int U = 0, H = 1, D = 2;

    /* first step: U or H are allowed (D would go negative) */
    cur[1 * 3 + U] = (seq_t)1;   /* start with U: altitude 1 */
    cur[0 * 3 + H] = (seq_t)1;   /* start with H: altitude 0 */

    /* iterate over steps 2 .. N */
    for (size_t step = 2; step <= N; ++step) {
        /* zero the part of 'next' that will be used */
        size_t used_next = step * 3;          /* max new height is 'step' */
        for (size_t i = 0; i < used_next; ++i) next[i] = (seq_t)0;

        size_t max_h = step - 1;             /* highest altitude before step */
        for (size_t h = 0; h <= max_h; ++h) {
            size_t base = h * 3;
            seq_t valU = cur[base + U];
            seq_t valH = cur[base + H];
            seq_t valD = cur[base + D];

            /* transitions from last step U: U->U and U->D */
            next[(h + 1) * 3 + U] += valU;           /* U->U */
            if (h > 0) next[(h - 1) * 3 + D] += valU;/* U->D */

            /* transitions from last step H: H->U */
            next[(h + 1) * 3 + U] += valH;           /* H->U */

            /* transitions from last step D: D->H and D->D */
            next[h * 3 + H] += valD;                 /* D->H */
            if (h > 0) next[(h - 1) * 3 + D] += valD;/* D->D */
        }

        /* swap current and next arrays */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* sum all ways that end at altitude 0 (height 0) */
    seq_t ans = cur[0 * 3 + U] + cur[0 * 3 + H] + cur[0 * 3 + D];

    free(cur);
    free(next);
    return ans;
}
