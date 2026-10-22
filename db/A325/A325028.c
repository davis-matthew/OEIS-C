/*
 * A325028  Triangle read by rows: T(n,k), 0 <= k < n, is the number of
 *          intervals [a,a+1) or [ma,m(a+1)) that must be XORed together to
 *          form the interval [k,n), where m = A325027(n,k).
 * https://oeis.org/A325028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A325028:  Triangle read by rows T(n,k) (0 ≤ k < n).
   Each element is the minimal number of intervals needed
   to XOR together to form [k,n).  The sequence a(n)
   is the flattened triangle, with a(1) = 1.  This
   implementation finds the appropriate row and column
   for the flattened index n, then searches every
   bin size m in 1..r, computing the value
   F(n,k,m) = u − v + |u*m−n| + |v*m−k| with
   u = ceil(n/m−½) and v = floor(k/m+½).  The
   minimum over m is returned.  The algorithm
   runs in O(r) per call; for n ≤ 10000,
   r ≤ 141, well below the time limit. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n (1‑based rows). */
    idx_t low = 0, high = 1;
    unsigned __int128 t;
    while ((((unsigned __int128)high) * (high + 1U)) / 2U < n) high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        t = (((unsigned __int128)mid) * (mid + 1U)) / 2U;
        if (t >= n) high = mid;
        else low = mid + 1;
    }
    idx_t r = low;                            /* row number (1‑based) */

    /* Previous triangular number and column index k (0‑based). */
    unsigned __int128 prevTri = (((unsigned __int128)r) * (r - 1)) / 2U;
    idx_t k = n - (idx_t)prevTri - 1;          /* column index (0‑based) */

    /* Find the minimal F over all m = 1..r. */
    seq_t best = 0;
    int first = 1;
    for (idx_t m = 1; m <= r; ++m) {
        /* u = ceil(r/m - ½) = (2*r + m - 1) / (2*m)  */
        idx_t u = (2*r + m - 1) / (2*m);
        /* v = floor(k/m + ½) = (k + floor(m/2)) / m  */
        idx_t v = (k + (m >> 1)) / m;

        seq_t um = (seq_t)u * (seq_t)m;
        seq_t vm = (seq_t)v * (seq_t)m;

        seq_t diff1 = um > (seq_t)r ? um - (seq_t)r : (seq_t)r - um;
        seq_t diff2 = vm > (seq_t)k ? vm - (seq_t)k : (seq_t)k - vm;
        int64_t temp = (int64_t)u - (int64_t)v + (int64_t)diff1 + (int64_t)diff2;
        seq_t F = (seq_t)temp;

        if (first || F < best) {
            best = F;
            first = 0;
        }
    }
    return best;
}
