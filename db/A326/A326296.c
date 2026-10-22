/*
 * A326296  Triangle of numbers T(n,k) = 2*floor(k/2)*(n-k) +
 *          ceiling((k-1)^2/2), 1<=k<=n.
 * https://oeis.org/A326296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 294, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A326296 – Triangle of numbers
 *
 *  Definition:
 *     T(n, k) = 2*floor(k/2)*(n-k) + ceil((k-1)²/2)   , 1 ≤ k ≤ n
 *
 *  The sequence a(n) is the triangle read row by row:
 *     a(1) = T(1,1)
 *     a(2) = T(2,1), a(3) = T(2,2)
 *     a(4) = T(3,1), a(5) = T(3,2), a(6) = T(3,3)
 *     …
 *
 *  For a given index n (1‑based) we first find the row r such that
 *
 *     r*(r-1)/2 <  n ≤ r*(r+1)/2
 *
 *  Then the column is
 *
 *     k = n - r*(r-1)/2
 *
 *  Finally a(n) = T(r, k).
 *
 *  All arithmetic is done with the seq_t type; the idx_t type is
 *  used only for indices.
 *
 *  Complexity: O(log r) per call, which is < 20 steps for all 64‑bit
 *  indices.  No floating‑point operations are used, so the function
 *  works for arbitrary‑precision seq_t.
 */

static inline idx_t row_of(idx_t n)
{
    /* Find the smallest r such that r*(r+1)/2 ≥ n. */
    idx_t low = 1;
    idx_t high = 1;

    /* Exponential growth to bound the search interval. */
    while ((unsigned __int128)high * (high + 1u) / 2u < n)
        high <<= 1;          /* high *= 2 */

    /* Binary search in [low, high]. */
    low = high >> 1;         /* previous high that was too small */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 cum = (unsigned __int128)mid * (mid + 1u) / 2u;
        if (cum >= n)
            high = mid;
        else
            low = mid + 1u;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0u)               /* safety: seq starts at 1 */
        return (seq_t)0;

    /* Locate the row r and column k corresponding to index n. */
    idx_t r = row_of(n);
    unsigned __int128 before = (unsigned __int128)(r - 1u) * r / 2u;
    idx_t k = n - (idx_t)before;

    /* Compute T(r, k) using seq_t arithmetic. */
    seq_t r_s = (seq_t)r;
    seq_t k_s = (seq_t)k;
    seq_t k_half = k_s / 2u;                      /* floor(k/2)        */
    seq_t term1 = (seq_t)2u * k_half * (r_s - k_s); /* 2*floor(k/2)*(r-k) */

    seq_t kk1 = k_s - 1u;                          /* k-1               */
    seq_t num = kk1 * kk1;                         /* (k-1)²            */
    seq_t term2 = (num + 1u) / 2u;                 /* ceil((k-1)²/2)    */

    return term1 + term2;
}
