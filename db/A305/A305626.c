/*
 * A305626  Number of chiral pairs of rows of n colors with exactly 6
 *          different colors.
 * https://oeis.org/A305626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6! / 2) * (S2(n,6) - S2(ceil(n/2),6))
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* S2(n,k) = Stirling numbers of the second kind.            */

#include <stdlib.h>

/* Persistent cache of S2(n,6)  for all n up to the largest n
   requested.  The cache is built incrementally so that calls   */
/* to a() in any order are O(n) for the first call that needs   */
/* a new n, all later calls are O(1).                            */
/* `row` holds the Stirling numbers for the current n.          */
static seq_t *S6 = NULL;      /* S6[n] = S2(n,6)   */
static idx_t S6_max = 0;      /* largest n computed  */
static seq_t row[7];          /* Stirling numbers for the current n */
static seq_t newrow[7];       /* helper for next n */

/* Ensure that S6[0..up_to] are computed.  The function builds   */
/* the array incrementally using the recurrence                   */
static void ensure_S6(idx_t up_to)
{
    if (S6 == NULL) {                    /* first call */
        S6 = (seq_t*)malloc((up_to + 1) * sizeof(seq_t));
        if (S6 == NULL) return;          /* allocation failure – should not happen */
        for (int k = 0; k <= 6; ++k) row[k] = 0;
        row[0] = 1;                      /* S(0,0)=1, all others 0 */
        S6[0] = 0;                       /* S(0,6)=0          */
        S6_max = 0;
    }
    if (up_to <= S6_max) return;         /* already computed */

    /* enlarge cache if necessary */
    seq_t *tmp = (seq_t*)realloc(S6, (up_to + 1) * sizeof(seq_t));
    if (tmp == NULL) return;            /* allocation failure – should not happen */
    S6 = tmp;

    for (idx_t n = S6_max + 1; n <= up_to; ++n) {
        newrow[0] = 0;
        for (int k = 1; k <= 6; ++k) {
            newrow[k] = row[k-1] + row[k] * (seq_t)k;   /* S(n,k) = k*S(n-1,k)+S(n-1,k-1) */
        }
        /* copy new row into row for next iteration */
        for (int k = 0; k <= 6; ++k) row[k] = newrow[k];
        S6[n] = row[6];               /* store S(n,6) */
    }
    S6_max = up_to;
}

/* The sequence a(n).  The function may be called with values in  */
/* any order; the internal cache guarantees that each n is only   */
/* computed once.                                                  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined, but harmless */
    ensure_S6(n);

    seq_t s_n     = S6[n];
    idx_t half    = (n + 1) / 2;        /* ceil(n/2) */
    seq_t s_half  = S6[half];

    seq_t diff    = s_n - s_half;       /* S(n,6) - S(ceil(n/2),6) */
    seq_t result  = diff * ((seq_t)360); /* 6! / 2 = 720 / 2 = 360 */

    return result;
}
