/*
 * A326115  Number of maximal double-free subsets of {1..n}.
 * https://oeis.org/A326115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 53, u32 n <= 103, u64 n <= 205,
 *            u128 n <= 409, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A326115 Sequence Number of maximal double‑free subsets of {1…n} 
	Formula implemented: 
	    a(n) = ∏_{odd k ≤ n} M(⌊log₂(n/k)⌋ + 1)

	Where M(m) is the number of maximal independent sets in a path of m vertices:
	    M(1) = 1, M(2) = 2, M(3) = 2, and for m ≥ 4, M(m) = M(m‑2) + M(m‑3).
*/

#include <stdint.h>
#include <stdbool.h>

/* compute floor(log2(x)) for x>0 */
static inline unsigned floor_log2_uint64(idx_t x)
{
    unsigned r = 0;
    while (x >>= 1) ++r;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* maximum length of a chain is floor(log2(n)) + 1 */
    unsigned log_n = floor_log2_uint64(n);
    idx_t maxL = log_n + 1;           /* max chain length (m ≥ 1) */

    /* precompute M[1..maxL] */
    seq_t M[70];                      /* 70 > maximum possible m (≤ 64) */
    M[0] = 0;                         /* unused */
    M[1] = 1;
    if (maxL >= 2) M[2] = 2;
    if (maxL >= 3) M[3] = 2;
    for (idx_t m = 4; m <= maxL; ++m)
        M[m] = M[m - 2] + M[m - 3];

    seq_t prod = 1;
    for (idx_t k = 1; k <= n; k += 2)          /* iterate over odd k */
    {
        idx_t t = n / k;                       /* n/(2k+1) for odd k = 2m+1 */
        unsigned logt = floor_log2_uint64(t);   /* floor(log2(t)) */
        idx_t L = logt + 1;                    /* chain length for this odd base */
        prod *= M[L];
    }

    return prod;
}
