/*
 * A292999  Triangle read by rows: T(n,k) (n >= 1, 4 <= k <= n+3) is the
 *          number of k-sequences of balls colored with at most n colors such
 *          that exactly four balls are the same color as some other ball in
 *          the sequence.
 * https://oeis.org/A292999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) with r the row for which r(r+1)/2 ≥ n, k = 3 + n -
 *            (r-1)r/2, and T(r,k) = binomial(k,4) * [ r!/(r+3-k)! + 3 *
 *            r!/(r+2-k)! ]
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    /* Find the row r such that r*(r+1)/2 ≥ n. */
    idx_t low  = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 sum = (unsigned __int128)mid * (mid + 1) / 2;
        if (sum >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r  = low;
    idx_t prev = (idx_t)((unsigned __int128)(r - 1) * r / 2);
    idx_t in_row = n - prev;          /* 1‑based position within this row */
    idx_t k = 3 + in_row;             /* column index */

    seq_t bin = binomial(k, 4);

    /* Compute product1 = r! / (r+3-k)! : product of i from (r+4-k) to r. */
    seq_t prod1 = 1;
    idx_t start1 = r + 4 - k;
    for (idx_t i = start1; i <= r; ++i)
        prod1 *= (seq_t)i;

    /* Compute product2 = r! / (r+2-k)! : product of i from (r+3-k) to r. */
    seq_t prod2 = 0;
    if (k <= r + 2) {
        seq_t prod = 1;
        idx_t start2 = r + 3 - k;
        for (idx_t i = start2; i <= r; ++i)
            prod *= (seq_t)i;
        prod2 = prod;
    }

    seq_t sum = prod1 + 3 * prod2;
    return bin * sum;
}
