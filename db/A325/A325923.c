/*
 * A325923  Number of Motzkin meanders of length n with an odd number of humps
 *          and an even number of peaks.
 * https://oeis.org/A325923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of Motzkin meanders of length n with an odd number of
 *            humps and an even number of peaks
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm performs a dynamic programming over the length of the
   meander, keeping track of the current height, the type of the previous
   step, whether we are inside a potential hump, and the parities of
   peaks and humps.  The state space is therefore small (24 states per
   height), and the DP runs in O(n^2).  This is fast enough for n<=~10^4. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define IDX(prev,inside,pPeak,pHump) (((((prev)*2 + (inside)))*2 + (pPeak))*2 + (pHump))

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* a(0) = 0 */

    const size_t totalStates = 24;  /* 3*2*2*2 */
    const size_t heightSize = (size_t)n + 1;
    const size_t cap = heightSize * totalStates;

    /* Allocate DP tables */
    seq_t *curr = (seq_t *)calloc(cap, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(cap, sizeof(seq_t));
    if (!curr || !next) return 0;   /* allocation failure safety */

    /* Initial state: height 0, previous step treated as D (no peak/hump),
       not inside a hump, even peaks, even humps. */
    curr[IDX(2,0,0,0)] = 1;

    for (size_t step = 0; step < (size_t)n; ++step) {
        /* Clear next table */
        memset(next, 0, cap * sizeof(seq_t));

        /* Current maximum height reachable after 'step' steps */
        size_t maxH = step;
        for (size_t h = 0; h <= maxH; ++h) {
            size_t base = h * totalStates;
            for (uint8_t s = 0; s < totalStates; ++s) {
                seq_t cnt = curr[base + s];
                if (!cnt) continue;

                uint8_t prev   = s / 8;          /* 0=U,1=H,2=D */
                uint8_t inside = (s % 8) / 4;    /* 0/1 */
                uint8_t pPeak  = (s % 4) / 2;    /* parity of peaks */
                uint8_t pHump  = s % 2;          /* parity of humps */

                /* Step U */
                size_t hU   = h + 1;
                next[hU * totalStates + IDX(0,1,pPeak,pHump)] += cnt;

                /* Step H */
                next[h * totalStates + IDX(1,inside,pPeak,pHump)] += cnt;

                /* Step D (only if h>0) */
                if (h > 0) {
                    size_t hD   = h - 1;
                    uint8_t pPeak2 = pPeak;
                    if (prev == 0) pPeak2 ^= 1;      /* peak if previous was U */
                    uint8_t pHump2 = pHump;
                    if (inside == 1) pHump2 ^= 1;    /* end a hump if inside */
                    next[hD * totalStates + IDX(2,0,pPeak2,pHump2)] += cnt;
                }
            }
        }

        /* Swap tables for next iteration */
        seq_t *tmp = curr;
        curr = next;
        next = tmp;
    }

    /* Sum over all terminal states with odd hurms (pHump=1) and even peaks (pPeak=0) */
    seq_t result = 0;
    for (size_t h = 0; h <= (size_t)n; ++h) {
        size_t base = h * totalStates;
        for (uint8_t prev = 0; prev < 3; ++prev)
            for (uint8_t inside = 0; inside < 2; ++inside)
                result += curr[base + IDX(prev,inside,0,1)];
    }

    /* Clean up */
    free(curr);
    free(next);

    return result;
}
