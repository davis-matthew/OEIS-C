/*
 * A334467  Square array read by antidiagonals upwards: T(n,k) is the sum of
 *          all parts of all partitions of n into consecutive parts that
 *          differ by k, with n >= 1, k >= 0.
 * https://oeis.org/A334467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = n * A323345(n,k) * where (n,k) is obtained from the index
 *            n by reading the * infinite square array T(n,k)=n*A323345(n,k)
 *            along * anti‑diagonals upwards (bottom to top)
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard integer types */
#include <stdint.h>
#include <stdbool.h>

/* ----- helper: integer square root (floor) for idx_t -----
 * Computes floor(sqrt(x)) without floating point.       */
static idx_t isqrt_idx(idx_t x)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= x)
        ++r;
    return r;
}

/* ----- helper: find diagonal index s for a given position -----
 * Returns the smallest s such that s*(s+1)/2 >= idx.   */
static idx_t find_s(idx_t idx)
{
    /* simple linear search – O(sqrt(idx)) is fine for idx ≤ 10 000   */
    idx_t s = 0;
    while ((s * (s + 1)) / 2 < idx)
        ++s;
    return s;
}

/* ----- count_partitions(n,k) -----
 * Count the number of partitions of n into consecutive parts
 * that differ by k (n ≥ 1, k ≥ 0).  */
static seq_t count_partitions(idx_t n, idx_t k)
{
    /* k == 0 : parts equal – count divisors of n */
    if (k == 0) {
        seq_t cnt = 0;
        idx_t r = isqrt_idx(n);
        for (idx_t i = 1; i <= r; ++i) {
            if (n % i == 0) {
                cnt += 2;          /* i and n/i */
            }
        }
        if (r * r == n)           /* perfect square counted twice */
            cnt -= 1;
        return cnt;
    }

    /* k > 0 : iterate over possible lengths m */
    seq_t cnt = 0;
    for (idx_t m = 1; ; ++m) {
        /* term = k * m * (m-1) / 2, computed in __int128 to avoid overflow */
        unsigned __int128 prod = (unsigned __int128)k * m * (m - 1);
        idx_t term = (idx_t)(prod / 2);
        if (term >= n)
            break;
        idx_t numer = n - term;
        if (numer % m == 0) {
            idx_t a = numer / m;     /* a ≥ 1 automatically for m ≥ 1 */
            if (a >= 1)
                ++cnt;
        }
    }
    return cnt;
}

/* ----- main sequence function -----
 * a(idx_t n) returns the n-th term (1‑based). */
seq_t a(idx_t n)
{
    /* Map the flat index to (row, col) in the array */
    idx_t s = find_s(n);                     /* diagonal number */
    idx_t prev = (s - 1) * s / 2;            /* number of terms before this diagonal */
    idx_t pos  = n - prev;                   /* 1‑based position inside diagonal */
    idx_t row = s - pos + 1;                 /* row = n */
    idx_t col = pos - 1;                     /* col = k */

    seq_t part_cnt = count_partitions(row, col);
    return (seq_t)row * part_cnt;
}
