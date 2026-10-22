/*
 * A324225  Total number T(n,k) of 1's in falling diagonals with index k in
 *          all n X n permutation matrices; triangle T(n,k), n>=1,
 *          1-n<=k<=n-1, read by rows.
 * https://oeis.org/A324225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   a(n) is the n‑th element of the flattened triangle * T(r,k) =
 *            (r-|k|)*(r-1)! (¡-diagonals in all r×r permutation matrices), *
 *            where r is the smallest integer with r² ≥ n. * a(n) =
 *            (r-|k|)*(r-1)! with k = (n-(r-1)²) - r. * (See OEIS sequence
 *            A324225 – “Total number of 1’s in falling diagonals”.)
 * Range:     u8 n <= 27, u16 n <= 65, u32 n <= 152, u64 n <= 407,
 *            u128 n <= 1157, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

typedef uint64_t idx_t;          /* defined by the build system */

/* Integer square root: floor(sqrt(x)) for unsigned 64‑bit x.        */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    idx_t bit = (idx_t)1 << 62;                /* highest power of four <= 2⁶⁴-1 */
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= r + bit) {                 /* no overflow: r+bit ≤ x ≤ 2⁶⁴-1   */
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* Memoised factorial array for (r-1)!  (r ≥ 1).  Elements are of type seq_t. */
static seq_t *fact = NULL;
static size_t factSize = 0;

seq_t a(idx_t n)
{
    /* Determine row r: r = ceil(sqrt(n)) = floor_sqrt(n-1)+1                    */
    idx_t r = isqrt_u64(n - 1) + 1;                   /* r ≥ 1 */

    /* Compute offset within row: 1‑based index in the row               */
    unsigned __int128 prevSq = (unsigned __int128)(r - 1) * (r - 1);
    unsigned __int128 tmp   = (unsigned __int128)n - prevSq;   /* 1 … 2r-1           */
    seq_t        offset = (seq_t)tmp;                        /* 1‑based offset     */

    seq_t pos0 = offset - 1;                                 /* 0‑based position   */
    int64_t k   = (int64_t)pos0 - (int64_t)(r - 1);           /* signed diagonal   */
    idx_t  t    = (k < 0) ? (idx_t)(-k) : (idx_t)k;          /* |k|               */

    /* Ensure factorial array contains (r-1)!                            */
    if (factSize == 0) {                 /* first call */
        fact = (seq_t *)malloc(sizeof(seq_t));
        if (!fact) abort();
        fact[0] = 1;
        factSize = 1;
    }
    if (factSize <= r - 1) {              /* extend array   */
        size_t newSize = (size_t)r;
        seq_t *tmpFact = (seq_t *)realloc(fact, newSize * sizeof(seq_t));
        if (!tmpFact) abort();
        for (idx_t i = factSize; i < r; ++i) {
            tmpFact[i] = tmpFact[i - 1] * (seq_t)i;
        }
        fact = tmpFact;
        factSize = newSize;
    }

    /* Final result: (r - |k|) * (r-1)!                                 */
    seq_t coeff = factSize ? (seq_t)(r - t) : 1; /* r - t is always ≥1   */
    return fact[r - 1] * coeff;
}
