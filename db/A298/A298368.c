/*
 * A298368  Triangle read by rows: T(n, k) =
 *          floor((n-1)/2)*floor(n/2)*floor((k-1)/2)*floor(k/2).
 * https://oeis.org/A298368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl, nice
 * Formula:   a(n) = T(r,c) where
 *            T(r,c)=floor((r-1)/2)*floor(r/2)*floor((c-1)/2)*floor(c/2) * r,c
 *            are obtained by row‑major indexing of the triangle: * find r
 *            such that r(r+1)/2 >= n > (r-1)r/2, then c = n - (r-1)r/2
 * Range:     u8 n <= 44, u16 n <= 560, u32 n <= 9554, u64 n <= 9554,
 *            u128 n <= 9554, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r that contains the n-th element. */
    idx_t low = 1, high = 1;
    /* Increase high until it is large enough. */
    while ((seq_t)high * (high + 1) / 2 < n) {
        high <<= 1;
    }
    /* Binary search for minimal r with r(r+1)/2 >= n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((seq_t)mid * (mid + 1) / 2 >= n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    idx_t r = low;

    /* Compute the column c within that row. */
    idx_t before = (idx_t)((seq_t)(r - 1) * r / 2);
    idx_t c = n - before;

    /* Compute the triangle entry. */
    seq_t a1 = (seq_t)(r - 1) / 2; /* floor((r-1)/2) */
    seq_t a2 = (seq_t)r / 2;        /* floor(r/2)     */
    seq_t b1 = (seq_t)(c - 1) / 2; /* floor((c-1)/2) */
    seq_t b2 = (seq_t)c / 2;        /* floor(c/2)     */

    return a1 * a2 * b1 * b2;
}
