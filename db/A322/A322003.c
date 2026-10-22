/*
 * A322003  Indices where A028897(A322000(n)) increases. Partial sums of
 *          A072170(n,10).
 * https://oeis.org/A322003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0}^{n-1} A072170(k,10)
 * Range:     u8 n <= 19, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* cache for decibinary representation counts */
static seq_t *w       = NULL;   /* w[i] = A072170(i,10) */
static idx_t wCap     = 0;     /* size of w array (>= limit+1) */

/* prefix sums of w: pref[i] = sum_{k< i} w[k]  */
static seq_t *pref    = NULL;   /* pref[i] = sum_{k< i} w[k] */
static idx_t prefCap  = 0;     /* size of pref array (>= wCap+1) */

/* Ensure that w and pref are computed up to 'limit' (inclusive on w). */
static void ensure_up_to(idx_t limit)
{
    if (limit < wCap) return;            /* already computed */

    idx_t newCap = limit + 1;            /* indices 0..limit */

    /* Temporary DP arrays – cast void* to seq_t* to satisfy C++ */
    seq_t *curr = (seq_t *)calloc(newCap, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(newCap, sizeof(seq_t));
    if (!curr || !next) {
        free(curr); free(next);
        return;                 /* allocation failure – abort silently */
    }

    curr[0] = (seq_t)1;                 /* base case: zero digits give 0 */

    /* Determine number of positions needed */
    idx_t pos = 0;
    while (((idx_t)1 << pos) <= limit) pos++;

    for (idx_t p = 0; p < pos; ++p) {
        idx_t weight = ((idx_t)1 << p);
        memset(next, 0, newCap * sizeof(seq_t));

        for (idx_t old = 0; old <= limit; ++old) {
            seq_t cnt = curr[old];
            if (!cnt) continue;
            for (int d = 0; d <= 9; ++d) {
                idx_t newVal = old + d * weight;
                if (newVal > limit) break;
                next[newVal] += cnt;
            }
        }

        /* Swap arrays for next iteration */
        seq_t *tmp = curr;
        curr = next;
        next = tmp;
    }

    /* Copy results into global w array */
    w = (seq_t *)realloc(w, newCap * sizeof(seq_t));
    for (idx_t i = 0; i <= limit; ++i) w[i] = curr[i];
    wCap = newCap;

    free(curr);
    free(next);

    /* Build prefix sums */
    pref = (seq_t *)realloc(pref, (newCap + 1) * sizeof(seq_t));
    pref[0] = (seq_t)0;
    for (idx_t i = 0; i < newCap; ++i) {
        pref[i + 1] = pref[i] + w[i];
    }
    prefCap = newCap + 1;
}

/* Public function */
seq_t a(idx_t n)
{
    if (n == 0ull) return (seq_t)0;
    if (prefCap <= n)            /* need pref[n] -> compute up to n-1 */
        ensure_up_to(n - 1);
    return pref[n];
}
