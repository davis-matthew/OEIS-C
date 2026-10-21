/*
 * A273144  Number of active (ON, black) cells in n-th stage of growth of
 *          two-dimensional cellular automaton defined by "Rule 597", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n‑2) - 2*a(n‑6) + a(n‑8) (for n > 7) Initial values:
 *            a(0)=1, a(1)=8, a(2)=20, a(3)=41, a(4)=57, a(5)=104, a(6)=116,
 *            a(7)=193 (OEIS A273144 – Rule 597 active cells)
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* return a base value directly if no recurrence is needed */
    if (n < 8)
    {
        switch (n)
        {
            case 0u: return (seq_t)1;
            case 1u: return (seq_t)8;
            case 2u: return (seq_t)20;
            case 3u: return (seq_t)41;
            case 4u: return (seq_t)57;
            case 5u: return (seq_t)104;
            case 6u: return (seq_t)116;
            case 7u: return (seq_t)193;
            default: return (seq_t)0;   /* should never happen */
        }
    }

    /* allocate space for the recurrence values */
#ifdef __cplusplus
    std::vector<seq_t> v((size_t)(n + 1));
#else
    seq_t *v = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!v) return (seq_t)0;  /* out of memory – unlikely */
#endif

    /* initialise the first eight terms */
    v[0] = (seq_t)1;
    v[1] = (seq_t)8;
    v[2] = (seq_t)20;
    v[3] = (seq_t)41;
    v[4] = (seq_t)57;
    v[5] = (seq_t)104;
    v[6] = (seq_t)116;
    v[7] = (seq_t)193;

    /* compute the remaining terms up to n using the recurrence */
    for (idx_t i = 8; i <= n; ++i)
    {
        size_t ii = (size_t)i;
        v[ii] = (seq_t)( (seq_t)2 * v[ii-2]
                        - (seq_t)2 * v[ii-6]
                        + v[ii-8] );
    }

    /* fetch result, free temporary storage if we used malloc */
    seq_t result = v[(size_t)n];
#ifndef __cplusplus
    free(v);
#endif

    return result;
}
