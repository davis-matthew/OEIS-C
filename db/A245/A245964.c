/*
 * A245964  Triangle read by rows: T(n,k) is the number of maximal hypercubes
 *          Q(p) in the Lucas cube Lambda(n) (i.e., Q(p) is an induced
 *          subgraph of Lambda(n) that is not a subgraph of a subgraph of
 *          Lambda(n) that is isomorphic to the hypercube Q(p+1)).
 * https://oeis.org/A245964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A245964: Triangle read by rows
   T(n,k) = (n/k) * binomial(k, n-2k)   (k ≥ 1)
   a(n) is the n‑th term when the triangle is read row‑wise (offset 0). */

#include "../utils/binomial.h"

/* Total number of entries up to (and including) row R. */
static inline idx_t cumulative_terms(idx_t R)
{
    idx_t M = R / 2;
    if ((R & 1) == 0)          /* R even:  R = 2M   */
        return (R + 1) + M * M; /* (R+1)+M² */
    else                        /* R odd :  R = 2M+1 */
        return (R + 1) + M * (M + 1); /* (R+1)+M(M+1) */
}

seq_t a(idx_t n)
{
    /* Base cases for the first two rows (k = 0 entries). */
    if (n <= 1)
        return 1;

    /* Find the row r that contains the n-th element. */
    idx_t low  = 0;
    idx_t high = n;                /* high is guaranteed to be big enough */
    while (low < high) {
        idx_t mid = (low + high) >> 1;      /* mid = (low+high)/2 */
        idx_t cum = cumulative_terms(mid);
        if (cum > n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                     /* row index (r ≥ 2 for n ≥ 2) */

    /* Position of the element inside its row. */
    idx_t prev_cum = cumulative_terms(r - 1);
    idx_t offset   = n - prev_cum;     /* 0 … floor(r/2) */

    if (offset == 0) {                 /* the k = 0 entry of the row */
        return (r < 2) ? 1 : 0;        /* T(0,0)=T(1,0)=1, otherwise 0 */
    }

    idx_t k = offset;                  /* column index (k ≥ 1) */

    if (r < 2 * k)                      /* binomial argument negative */
        return 0;

    idx_t m   = r - 2 * k;              /* = n-2k in the formula */
    seq_t bin = binomial(k, m);         /* C(k, n-2k) */
    if (bin == 0)
        return 0;

    /* Compute (n/k) * binomial(k, n-2k) as (n * binomial) / k. */
    seq_t res = ((seq_t)r * bin) / (seq_t)k;
    return res;
}
