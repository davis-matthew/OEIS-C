/*
 * A279063  Number of 2 X 2 matrices having entries in {0,1,...,n} and
 *          determinant in the closed interval [-n,n] with no entry repeated.
 * https://oeis.org/A279063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 2×2 matrices with entries in {0,…,n}, distinct
 *            entries and determinant in [‑n,n]. The algorithm builds the
 *            frequency array of ordered distinct pairs (i,j) with product
 *            i*j, then uses a sliding window to count all pairs (a,d),(b,c)
 *            with determinant in range. From this total it sub...
 * Range:     u8 n <= 5, u16 n <= 28, u32 n <= 150, u64 n <= 150,
 *            u128 n <= 150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* Convert index to normal signed integers for loops */
    int n_int = (int)n;
    size_t N = (size_t)n_int + 1;            /* number of values */
    size_t maxProd = (size_t)n_int * (size_t)n_int;  /* maximal product */

    /* Frequency array of ordered distinct pairs (i,j) with product i*j */
    /* 16‑bit entries are enough: for product 0 we have 2*n ≤ 20000. */
    uint16_t *C = (uint16_t *)calloc(maxProd + 1, sizeof(uint16_t));
    if (!C) return (seq_t)0;           /* allocation failed */

    /* Build the frequency array */
    for (size_t a = 0; a < N; ++a) {
        for (size_t d = 0; d < N; ++d) {
            if (a == d) continue;
            C[a * d]++;                 /* a*d fits in size_t (≤1e8) */
        }
    }

    /* Compute M = Σ_{p,q} C[p]·C[q] where |p−q|≤n with p≠? (cross restriction ignored) */
    seq_t M = 0;
    seq_t windowSum = 0;
    size_t low = 0, high = 0;           /* [low, high) window bounds */
    for (size_t p = 0; p <= maxProd; ++p) {
        /* Expand window to the right: include indices up to p+n */
        size_t upperHigh = p + (size_t)n_int;
        if (upperHigh > maxProd) upperHigh = maxProd;
        while (high <= upperHigh) {
            windowSum += (seq_t)C[high];
            ++high;
        }
        /* Shrink window from the left: remove indices < p−n */
        size_t lowerLow = (p >= (size_t)n_int) ? p - (size_t)n_int : 0;
        while (low < lowerLow) {
            windowSum -= (seq_t)C[low];
            ++low;
        }
        /* Add contribution of product p */
        M += (seq_t)C[p] * windowSum;
    }

    /* Count bad quadruples where an entry repeats between the two ordered pairs.
       We use two categories: a==b (≡ a==c) and d==b (≡ d==c). */
    seq_t S_ab = 0;    /* counts of a==b (or a==c) valid pairs */
    seq_t S_cd = 0;    /* counts of d==b (or d==c) valid pairs */

    for (int a = 0; a <= n_int; ++a) {
        for (int d = 0; d <= n_int; ++d) {
            if (a == d) continue;

            /* a==b case: count c != a with |a*(d-c)| ≤ n */
            if (a == 0) {
                S_ab += (seq_t)n_int;          /* each d≠0 gives n choices for c (≠0) */
            } else {
                int t = n_int / a;
                int lower = d - n_int;
                if (lower < -t) lower = -t;
                int upper = d;
                if (upper > t) upper = t;
                if (lower <= upper) {
                    int cnt = upper - lower + 1;
                    int diff_a = d - a;
                    if (diff_a >= lower && diff_a <= upper) cnt--;
                    if (cnt > 0) S_ab += (seq_t)cnt;
                }
            }

            /* d==b case: count c != d with |d*(a-c)| ≤ n */
            if (d == 0) {
                S_cd += (seq_t)n_int;          /* each a≠0 gives n choices for c (≠0) */
            } else {
                int t = n_int / d;
                int lower = a - t;
                if (lower < 0) lower = 0;
                int upper = a + t;
                if (upper > n_int) upper = n_int;
                if (lower <= upper) {
                    int cnt = upper - lower + 1;
                    if (d >= lower && d <= upper) cnt--;
                    if (cnt > 0) S_cd += (seq_t)cnt;
                }
            }
        }
    }

    /* Double intersections: a==b & d==c  OR  a==c & d==b
       Each type has (n+1)*n tuples. */
    seq_t double_intersections = (seq_t)(n_int + 1) * (seq_t)n_int;

    /* Total bad quadruples */
    seq_t bad = 2 * S_ab + 2 * S_cd - 2 * double_intersections;

    seq_t result = M - bad;

    free(C);
    return result;
}
