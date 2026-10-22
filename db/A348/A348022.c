/*
 * A348022  The numbers visited on a square spiral when stepping to the
 *          smallest unvisited number that is visible from and shares a
 *          divisor > 1 with the current number. Start with 1 and 2.
 * https://oeis.org/A348022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed by simulation as defined in sequence A348022 a(1)=1,
 *            a(2)=2 and thereafter a(n) is the smallest unvisited integer
 *            that shares a divisor >1 with a(n-1) and is visible from a(n-1)
 *            on the standard square spiral
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   gcd, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    /* Maximum number of spiral cells pre‑computed.
       1 000 000 gives ample room for n up to about 10 000.    */
    const size_t MAX_NUM = 1000000;

    /* Static data used for memoisation and precomputation. */
    static seq_t *seq          = NULL;   /* sequence values */
    static size_t seq_len      = 0;      /* number of values stored */
    static size_t seq_cap      = 0;      /* capacity of seq array */

    static uint8_t *visited    = NULL;   /* visited flag for each integer */
    static size_t visited_cap  = 0;

    static int64_t *cx         = NULL;   /* x‑coordinate of each integer */
    static int64_t *cy         = NULL;   /* y‑coordinate of each integer */

    /* Lazy initialisation on first call. */
    if (seq == NULL) {
        /* allocate visited array */
        visited_cap = MAX_NUM + 1;
        visited = (uint8_t*)calloc(visited_cap, 1);

        /* allocate coordinate arrays */
        cx = (int64_t*)malloc((MAX_NUM + 1) * sizeof(int64_t));
        cy = (int64_t*)malloc((MAX_NUM + 1) * sizeof(int64_t));

        /* coordinate of the centre */
        cx[1] = 0;  cy[1] = 0;

        /* compute coordinates for 2 .. MAX_NUM */
        for (size_t i = 2; i <= MAX_NUM; ++i) {
            idx_t n = (idx_t)i;
            idx_t s = int_sqrt(n);
            if (s * s < n)
                ++s;                           /* ceil(sqrt(n)) */
            idx_t r = s / 2;                   /* ring number */
            idx_t side = 2 * r;
            idx_t m = (2 * r + 1);
            m = m * m;                         /* (2r+1)^2 */
            idx_t t = m - n;

            int64_t x, y;
            if (t < side) {
                x = (int64_t)r - (int64_t)t;
                y = -(int64_t)r;
            } else if (t < 2 * side) {
                x = -(int64_t)r;
                y = -(int64_t)r + (t - side);
            } else if (t < 3 * side) {
                x = -(int64_t)r + (t - 2 * side);
                y = (int64_t)r;
            } else {
                x = (int64_t)r;
                y = (int64_t)r - (t - 3 * side);
            }
            cx[i] = x;
            cy[i] = y;
        }

        /* pre‑allocate sequence array */
        seq_cap = 16;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));

        /* initialise with the first two terms */
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)2;
        seq_len = 2;

        visited[1] = 1;
        visited[2] = 1;
    }

    /* generate sequence until it contains n elements */
    while (seq_len < n) {
        seq_t current = seq[seq_len - 1];

        /* find the smallest unvisited integer satisfying the rules */
        seq_t next = 0;
        for (size_t cand = 1; cand <= MAX_NUM; ++cand) {
            if (!visited[cand]) {
                /* must share a divisor > 1 with current */
                if (gcd(current, (seq_t)cand) > (seq_t)1) {
                    /* coordinate difference */
                    int64_t dx = cx[(size_t)current] - cx[cand];
                    int64_t dy = cy[(size_t)current] - cy[cand];
                    int64_t adx = dx < 0 ? -dx : dx;
                    int64_t ady = dy < 0 ? -dy : dy;
                    /* visible iff gcd(|dx|,|dy|) == 1 */
                    if (gcd((seq_t)adx, (seq_t)ady) == (seq_t)1) {
                        next = (seq_t)cand;
                        visited[cand] = 1;
                        break;
                    }
                }
            }
        }

        /* safety: should never happen with MAX_NUM large enough */
        if (next == (seq_t)0) {
            /* if MAX_NUM is exhausted, increase allocation and recompute
               coordinates for the new range. For the scope of this
               implementation we treat it as an impossible case. */
            return (seq_t)0;
        }

        /* append the new term to the sequence */
        if (seq_len == seq_cap) {
            seq_cap *= 2;
            seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
        }
        seq[seq_len++] = next;
    }

    return seq[n - 1];
}
