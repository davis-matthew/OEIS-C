/*
 * A384446  Triangle read by rows: T(n, k) = |gcd(n, k) - k|.
 * https://oeis.org/A384446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Trim: computes A384446 – Triangle read by rows
 *
 * Definition: T(n,k)=|gcd(n,k)-k|, offset 0.  The sequence a(n) is
 * obtained by flattening the triangle row‑wise:
 * a = T(0,0), T(1,0), T(1,1), T(2,0), T(2,1), T(2,2), …
 *
 * For a given linear index n we first find the row r such that
 * r(r+1)/2 ≤ n < (r+1)(r+2)/2.  Then k = n – r(r+1)/2.
 * The term equals k – gcd(r,k) (k==0 gives r).
 *
 * Complexity: O(log n) for locating the row, O(log r) for gcd.
 *   < 2 s for n up to 10⁴ or larger.
 */

#include <stdint.h>
#include <stdbool.h>

/* Euclidean gcd for unsigned integer arguments */
static idx_t gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    /* Find row r with r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t low = 0, high = n + 1;          /* upper bound inclusive */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;   /* mid = ceil((low+high)/2) */
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    idx_t start = (idx_t)(((__uint128_t)r * (r + 1)) / 2);
    idx_t k = n - start;

    if (k == 0) {
        return (seq_t)r;
    } else {
        idx_t g = gcd_idx(r, k);
        return (seq_t)(k - g);   /* k ≥ g, result fits into seq_t */
    }
}
