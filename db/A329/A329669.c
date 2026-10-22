/*
 * A329669  Number of meanders of length n with Motzkin-steps avoiding the
 *          consecutive steps HH and DD.
 * https://oeis.org/A329669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 25, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A329669 : Number of meanders of length n with Motzkin-steps
 * avoiding the consecutive steps HH and DD.
 *
 * Recurrence (dynamic programming):
 *   Let dpU[h], dpH[h], dpD[h] be the number of paths of current length
 *   ending at altitude h whose last step is U, H or D respectively.
 *
 *   Initial step (length 1):
 *     dpU[1] = 1   (U from altitude 0)
 *     dpH[0] = 1   (H from altitude 0)
 *     dpD[.] = 0   (no D from altitude 0)
 *
 *   Transition for each further step:
 *     From a U step at altitude h:
 *         nextU[h+1] += dpU[h]
 *         nextH[h]   += dpU[h]
 *         if (h>0)    nextD[h-1] += dpU[h]   // D allowed only if altitude > 0
 *
 *     From an H step at altitude h:
 *         nextU[h+1] += dpH[h]
 *         if (h>0)    nextD[h-1] += dpH[h]
 *
 *     From a D step at altitude h:
 *         nextU[h+1] += dpD[h]
 *         nextH[h]   += dpD[h]
 *
 *   After processing all n steps, a(n) is the sum over all h of
 *   dpU[h] + dpH[h] + dpD[h].
 *
 * The algorithm runs in O(n²) time and O(n) memory, which is well
 * within the 2‑second limit for n up to about 10 000.
 */

#include <stdlib.h>

/* External type definitions are provided by the build system */

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) return (seq_t)1;

    /* The maximum possible altitude after n steps is n */
    const size_t sz = (size_t)n + 1;

    /* Allocate arrays for current and next DP layers */
    seq_t *dpU   = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *dpH   = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *dpD   = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *nextU = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *nextH = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *nextD = (seq_t*)calloc(sz, sizeof(seq_t));

    if (!dpU || !dpH || !dpD || !nextU || !nextH || !nextD) {
        /* Allocation failed – return 0 as an undefined value */
        return (seq_t)0;
    }

    /* Initial step(s) */
    dpU[1] = (seq_t)1;   /* U from altitude 0 */
    dpH[0] = (seq_t)1;   /* H from altitude 0 */

    size_t cur_max = 1;  /* maximum altitude reached so far (after first step) */

    /* Process steps 2 .. n */
    for (idx_t step = 2; step <= n; ++step) {
        /* Zero next arrays up to possible altitude 'step' */
        for (size_t i = 0; i <= (size_t)step; ++i) {
            nextU[i] = nextH[i] = nextD[i] = (seq_t)0;
        }

        /* Apply transitions from the current layer */
        for (size_t h = 0; h <= cur_max; ++h) {
            seq_t vU = dpU[h];
            if (vU) {
                nextU[h + 1] += vU;          /* U -> U */
                nextH[h]     += vU;          /* U -> H */
                if (h > 0) nextD[h - 1] += vU; /* U -> D */
            }

            seq_t vH = dpH[h];
            if (vH) {
                nextU[h + 1] += vH;          /* H -> U */
                if (h > 0)   nextD[h - 1] += vH; /* H -> D */
            }

            seq_t vD = dpD[h];
            if (vD) {
                nextU[h + 1] += vD;          /* D -> U */
                nextH[h]     += vD;          /* D -> H */
            }
        }

        /* Swap current and next pointers */
        seq_t *tmp;
        tmp = dpU;  dpU  = nextU; nextU = tmp;
        tmp = dpH;  dpH  = nextH; nextH = tmp;
        tmp = dpD;  dpD  = nextD; nextD = tmp;

        cur_max = (size_t)step;   /* altitude can increase by at most one each step */
    }

    /* Sum all counts after n steps */
    seq_t result = (seq_t)0;
    for (size_t h = 0; h < sz; ++h) {
        result += dpU[h] + dpH[h] + dpD[h];
    }

    /* Free allocated memory */
    free(dpU); free(dpH); free(dpD);
    free(nextU); free(nextH); free(nextD);

    return result;
}
