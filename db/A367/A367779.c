/*
 * A367779  a(n) is the sum of the cubed areas under Motzkin paths of length
 *          n.
 * https://oeis.org/A367779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A367779: a(n) = sum of cubed areas under all Motzkin paths of length n.
   Computed by dynamic programming over the height level and cumulative
   area.  Not a closed‑form expression. */
#include <stdlib.h>
#include <string.h>

/* State for each height: number of prefixes, sum of areas,
   sum of area², sum of area³.  All statistics are of type seq_t. */
typedef struct {
    seq_t cnt;   /* number of prefixes */
    seq_t s1;    /* sum of areas              */
    seq_t s2;    /* sum of (area)²            */
    seq_t s3;    /* sum of (area)³            */
} State;

/* Merge a batch of prefixes into a destination state, adding the step
   height h to the area of each prefix. */
static void add_to_state(State *dst, seq_t cnt, seq_t s1, seq_t s2,
                         seq_t s3, seq_t h)
{
    dst->cnt += cnt;
    dst->s1  += s1 + cnt * h;
    dst->s2  += s2 + (seq_t)2 * h * s1 + (h * h) * cnt;
    dst->s3  += s3 + (seq_t)3 * h * s2 + (seq_t)3 * h * h * s1
               + (h * h * h) * cnt;
}

seq_t a(idx_t n)
{
    /* Empty path has area 0 */
    if (n == 0) return 0;

    size_t N = (size_t)n;             /* n ≤ ~10 000 in typical tests */
    State *cur  = (State *)calloc(N + 2, sizeof(State));
    State *next = (State *)calloc(N + 2, sizeof(State));
    if (!cur || !next) abort();       /* allocation failure */

    cur[0].cnt = 1;                    /* start at height 0, area 0 */

    for (size_t len = 0; len < N; ++len)
    {
        /* Clear next array for heights 0 … len+1 inclusive. */
        memset(next, 0, (len + 2) * sizeof(State));

        for (size_t h = 0; h <= len; ++h)
        {
            if (cur[h].cnt == 0) continue;

            seq_t cnt = cur[h].cnt;
            seq_t s1  = cur[h].s1;
            seq_t s2  = cur[h].s2;
            seq_t s3  = cur[h].s3;
            seq_t hval = (seq_t)h;

            /* Flat step: height stays h */
            add_to_state(&next[h], cnt, s1, s2, s3, hval);
            /* Up step: height becomes h+1 */
            add_to_state(&next[h + 1], cnt, s1, s2, s3, hval);
            /* Down step: allowed only if h > 0 */
            if (h > 0)
                add_to_state(&next[h - 1], cnt, s1, s2, s3, hval);
        }

        /* Prepare for next iteration */
        State *tmp = cur;
        cur = next;
        next = tmp;
    }

    seq_t result = cur[0].s3;  /* only paths that end at height 0 */
    free(cur);
    free(next);
    return result;
}
