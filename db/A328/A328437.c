/*
 * A328437  Number of inversion sequences of length n avoiding the consecutive
 *          pattern 001.
 * https://oeis.org/A328437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of inversion sequences of length n avoiding the *
 *            consecutive pattern 001. * * DP over the last two entries. For
 *            each length k (k ≥ 2) we store * two arrays of size k: * S[i] –
 *            number of sequences ending with (i,i) (last two equal) * D[i] –
 *            number of sequences ending with (x,i) wher...
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#if defined(__cplusplus)
#  include <vector>
#endif

/*==========================================================================*/
/*  Main function                                                          */
/*==========================================================================*/
seq_t a(idx_t n)
{
    /* handle small lengths */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* maximum index that may appear is n (value n-1 when length n) */
    const size_t N = (size_t)n;

#if defined(__cplusplus)
    /* use std::vector for automatic construction of seq_t (may be a class) */
    std::vector<seq_t> S(N + 1, 0), D(N + 1, 0);
    std::vector<seq_t> NS(N + 1, 0), ND(N + 1, 0);

    /* initial state for length 2: sequences (0,0) and (0,1) */
    S[0] = (seq_t)1;     /* (0,0) */
    D[1] = (seq_t)1;     /* (0,1) */

    /* iterate length len from 2 up to n-1 to build length len+1 */
    for (size_t len = 2; len < n; ++len) {
        /* compute total number of diff states for current length */
        seq_t total_diff = (seq_t)0;
        for (size_t i = 0; i < len; ++i)
            total_diff += D[i];

        /* suffix sum of S needed for new_D calculation */
        seq_t suffix = (seq_t)0;
        for (size_t z = len; z-- > 0; ) {
            /* new states with last value z (0 ≤ z < len) */
            NS[z] = S[z] + D[z];
            ND[z] = (total_diff - D[z]) + suffix;
            /* update suffix for next smaller z */
            suffix += S[z];
        }
        /* new maximum value z = len */
        NS[len] = (seq_t)0;
        ND[len] = total_diff;

        /* swap current and next arrays */
        std::swap(S, NS);
        std::swap(D, ND);
    }

    /* sum all entries for the final length n */
    seq_t result = (seq_t)0;
    for (size_t i = 0; i <= n; ++i)
        result += S[i] + D[i];
    return result;

#else
    /* C implementation with malloc / calloc */
    seq_t *S   = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *D   = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *NS  = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *ND  = (seq_t *)calloc(N + 1, sizeof(seq_t));

    if (!S || !D || !NS || !ND) {
        /* allocation failure – return 0 as a safe fallback */
        free(S); free(D); free(NS); free(ND);
        return (seq_t)0;
    }

    /* initial state for length 2 */
    S[0] = (seq_t)1;    /* (0,0) */
    D[1] = (seq_t)1;    /* (0,1) */

    for (size_t len = 2; len < n; ++len) {
        seq_t total_diff = (seq_t)0;
        for (size_t i = 0; i < len; ++i)
            total_diff += D[i];

        seq_t suffix = (seq_t)0;
        for (size_t z = len; z-- > 0; ) {
            NS[z] = S[z] + D[z];
            ND[z] = (total_diff - D[z]) + suffix;
            suffix += S[z];
        }
        NS[len] = (seq_t)0;
        ND[len] = total_diff;

        /* swap pointers */
        seq_t *tmp;
        tmp = S;  S = NS;  NS = tmp;
        tmp = D;  D = ND;  ND = tmp;
    }

    /* sum all entries for final length */
    seq_t result = (seq_t)0;
    for (size_t i = 0; i <= n; ++i)
        result += S[i] + D[i];

    free(S); free(D); free(NS); free(ND);
    return result;
#endif
}
