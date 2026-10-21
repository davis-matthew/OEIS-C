/*
 * A278987  Array read by antidiagonals downwards: T(b,n) = number of words of
 *          length n over an alphabet of size b that are in standard order and
 *          which have the property that every letter that appears in the word
 *          is repeated.
 * https://oeis.org/A278987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(b,n) = ∑ₖ₌₁^min(b,⌊n/2⌋) S₂(n,k) where S₂(n,k) is the
 *            number of partitions of an n‑element set into k non‑empty blocks
 *            each of size at least 2. They satisfy S₂(0,0)=1 , S₂(n,0)=0
 *            (n>0), and S₂(n,k) = k·S₂(n−1,k) + (n−1)·S₂(n−2,k−1) (n≥2). The
 *            OEIS index n is read by a “downward” antidiag...
 * Range:     u8 n <= 47, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   index_to_b_m
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/index_to_b_m.h"

/* Pre‑allocated tables for S₂ up to a fixed maximum */
#define MAX_N 1000          /* sufficient for all test cases */
static seq_t S2[MAX_N+1][MAX_N+1];
static idx_t S2_max = 0;

/* Build the table of S₂ values up to given N (inclusive) */
static void precompute_s2(idx_t N)
{
    if (N <= S2_max) return;

    /* initialise the base row */
    if (S2_max == 0) {
        S2[0][0] = 1;
        for (idx_t k = 1; k <= MAX_N; ++k) S2[0][k] = 0;
    }

    /* compute rows incrementally */
    for (idx_t n = S2_max + 1; n <= N; ++n) {
        S2[n][0] = 0;           /* cannot partition non‑empty set into 0 blocks */
        for (idx_t k = 1; k <= n; ++k) {
            seq_t term1 = 0;
            seq_t term2 = 0;
            if (k <= n-1)                /* k * S₂(n-1,k)  */
                term1 = k * S2[n-1][k];
            if (n >= 2 && k >= 1)        /* (n-1) * S₂(n-2,k-1) */
                term2 = (n-1) * S2[n-2][k-1];
            S2[n][k] = term1 + term2;
        }
    }

    S2_max = N;
}

/*  Public function required by the test harness  */
seq_t a(idx_t N)
{
    if (N < 1) return 0;                /* offset is 1 */

    idx_t b, m;
    index_to_b_m(N, &b, &m);

    if (m < 2) return 0;                /* words of length 1 cannot satisfy the repeat property */

    if (m > MAX_N)                      /* safety check – enlarge if needed */
        return 0;

    precompute_s2(m);

    idx_t maxk = m / 2;
    if (b < maxk) maxk = b;

    seq_t result = 0;
    for (idx_t k = 1; k <= maxk; ++k)
        result += S2[m][k];

    return result;
}
