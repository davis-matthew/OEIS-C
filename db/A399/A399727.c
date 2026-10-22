/*
 * A399727  Irregular triangular array read by rows. T(n,k) is the number of
 *          subsets S of [n] such that |S|*(n-|S|) = k, 0<=k<=A002620(n),
 *          n>=0.
 * https://oeis.org/A399727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, new
 * Formula:   a(n) is the nth term of the triangular array T(n,k) * where
 *            T(n,k) counts subsets S of [n] satisfying |S|·(n−|S|)=k, * read
 *            row‑by‑row (T(n,0),T(n,1),…,T(n,⌊n²/4⌋)). * The algorithm finds
 *            which row and column an index lies in, * then solves s²−n·s+k=0
 *            for the subset size s. For each integer * sol...
 * Range:     u8 n <= 41, u16 n <= 541, u32 n <= 3449, u64 n <= 9794,
 *            u128 n <= 9794, big any n
 * Helpers:   binomial, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Find the row r and column k such that the index n lies in that
     * position when the array is read row by row.  Each row r has
     * len(r)=⌊r²/4⌋+1 entries.                                     */
    idx_t idx = n;
    idx_t r = 0;
    while (1) {
        /* Integer division gives floor(r^2/4). */
        idx_t len = (r * r) / 4 + 1;
        if (idx < len)
            break;
        idx -= len;
        ++r;
    }
    idx_t k = idx;               /* column within row r */

    /* Compute T(r,k).  Solve s²−r·s+k=0.  Discriminant must be a
     * perfect square for integer solutions. */
    idx_t rhs = r * r - 4 * k;   /* guaranteed non‑negative for valid k */
    if (rhs > (idx_t)-1 / 4 * 4) /* sanity check: prevent overflow (never true here) */
        return 0;

    seq_t sqrt_seq = floor_sqrt((seq_t)rhs);
    idx_t sqrtD = (idx_t)sqrt_seq;
    if (sqrtD * sqrtD != rhs)   /* discriminant not a square */
        return 0;

    seq_t result = 0;

    /* First root: s1 = (r + sqrtD)/2 */
    if ( ((r + sqrtD) & 1) == 0 ) {
        idx_t s1 = (r + sqrtD) >> 1;
        result += binomial(r, s1);
    }

    /* Second root: s2 = (r − sqrtD)/2; skip if equal to s1 */
    if (sqrtD != 0 && ((r >= sqrtD) && (((r - sqrtD) & 1) == 0))) {
        idx_t s2 = (r - sqrtD) >> 1;
        result += binomial(r, s2);
    }

    return result;
}
