/*
 * A384047  Triangle read by rows: T(n, k) for 1 <= k <= n is the largest
 *          divisor of k that is a unitary divisor of n.
 * https://oeis.org/A384047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(row, k) where row = smallest integer with row*(row+1)/2
 *            >= n and k = n - (row-1)*row/2. T(row, k) = product of
 *            prime‑power factors p^e of row such that p^e divides k
 * Range:     u8 n <= 8574, u16 n <= 8574, u32 n <= 8574, u64 n <= 8574,
 *            u128 n <= 8574, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Helper: binary search for the row number */
static idx_t row_of(idx_t n)
{
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* Undefined for n==0, but never asked */

    /* Determine row and column */
    idx_t row = row_of(n);
    idx_t prev_rows = (row - 1) * row / 2;
    idx_t k = n - prev_rows;

    /* Find prime powers of row */
    uint64_t rem = (uint64_t)row;
    seq_t res = (seq_t)1;

    /* Handle factor 2 separately */
    if (rem % 2 == 0) {
        uint64_t power = 1;
        while (rem % 2 == 0) {
            rem /= 2;
            power *= 2;
        }
        if (k % power == 0) {
            res = (seq_t)((seq_t)res * (seq_t)power);
        }
    }

    /* Odd factors */
    for (uint64_t p = 3; p * p <= rem; p += 2) {
        if (rem % p == 0) {
            uint64_t power = 1;
            while (rem % p == 0) {
                rem /= p;
                power *= p;
            }
            if (k % power == 0) {
                res = (seq_t)((seq_t)res * (seq_t)power);
            }
        }
    }

    /* Remaining prime factor (if any) */
    if (rem > 1) {
        uint64_t power = rem;            /* rem is prime to power 1 */
        if (k % power == 0) {
            res = (seq_t)((seq_t)res * (seq_t)power);
        }
    }

    return res;
}
