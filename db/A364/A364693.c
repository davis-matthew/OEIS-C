/*
 * A364693  Characteristic function of polygonal numbers of order greater than
 *          2 (A090466): a(n) = 1 if n is in A090466, 0 otherwise.
 * https://oeis.org/A364693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a polygonal number of order > 2 with * index
 *            (the “k” in the polygonal number formula) ≥ 3, 0 otherwise
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The smallest polygonal number with index k ≥ 3 is 6 (k = 3, s = 3). */
    if (n < 6)
        return (seq_t)0;

    /* Iterate over k = index of the polygonal number.
     * The minimum possible value for a given k is the k‑th triangular number:
     *   T_k = k*(k+1)/2.
     * When T_k > n, no larger k can produce n, so we can stop. */
    for (idx_t k = 3;; ++k)
    {
        /* Compute T_k = k*(k+1)/2 using a 128‑bit integer to avoid overflow. */
        __int128 tri = (__int128)k * (k + 1) / 2;
        if (tri > n)
            break;

        /* For a fixed k, solve for the order s in the formula
         *      P(s,k) = k*((s-2)k - (s-4))/2 = n
         *      s = 2*(n + k(k-2)) / (k(k-1))
         */
        __int128 denom = (__int128)k * (k - 1);
        __int128 numer = 2 * ( (__int128)n + (__int128)k * (k - 2) );

        if (numer % denom == 0)
        {
            __int128 s = numer / denom;
            if (s >= 3)
                return (seq_t)1;
        }
    }
    return (seq_t)0;
}
