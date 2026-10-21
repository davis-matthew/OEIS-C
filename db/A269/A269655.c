/*
 * A269655  Number of length-n 0..7 arrays with no adjacent pair x,x+1
 *          repeated.
 * https://oeis.org/A269655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A269655
   Count of length‑n arrays over {0,…,7} in which each ordered pair (x,x+1)
   may appear at most once as consecutive values.  Indexing starts at 1. */

#include <stddef.h>
#include <string.h>

#define ALPH        8          /* alphabet size 0..7 */
#define PAIR_BITS   7          /* 7 possible consecutive pairs */
#define MASKS       (1U << PAIR_BITS)   /* 128 masks */
#define STATES      (ALPH * MASKS)      /* 1024 DP states */

/* a(n) : number of admissible arrays of length n (n >= 1) */
seq_t a(idx_t n)
{
    /* n == 0 is outside the offset; return 0 for safety */
    if (n == 0)
        return (seq_t)0;

    /* static storage for DP arrays */
    static seq_t cur_storage[STATES];
    static seq_t nxt_storage[STATES];
    seq_t *cur = cur_storage;
    seq_t *nxt = nxt_storage;

    /* initialise state for length 1: no pairs used yet */
    memset(cur, 0, STATES * sizeof(seq_t));
    memset(nxt, 0, STATES * sizeof(seq_t));
    for (idx_t last = 0; last < ALPH; ++last)
        cur[(size_t)last * MASKS] = (seq_t)1;

    /* DP from length 2 up to n */
    for (idx_t pos = 1; pos < n; ++pos) {
        memset(nxt, 0, STATES * sizeof(seq_t));

        for (idx_t last = 0; last < ALPH; ++last) {
            for (idx_t mask = 0; mask < MASKS; ++mask) {
                seq_t cnt = cur[(size_t)last * MASKS + mask];
                if (cnt == 0)
                    continue;

                /* try extending with each possible next element */
                for (idx_t next = 0; next < ALPH; ++next) {
                    if (last <= 6 && next == last + 1) {          /* pair (last,last+1) */
                        unsigned bit = 1U << (unsigned)last;
                        if (mask & bit)                         /* pair already used */
                            continue;
                        idx_t new_mask = mask | bit;
                        nxt[(size_t)next * MASKS + new_mask] += cnt;
                    } else {                                     /* any other transition */
                        nxt[(size_t)next * MASKS + mask] += cnt;
                    }
                }
            }
        }

        /* swap current and next arrays for the next iteration */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* sum over all states for the final result */
    seq_t total = (seq_t)0;
    for (size_t i = 0; i < STATES; ++i)
        total += cur[i];

    return total;
}
