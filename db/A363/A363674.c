/*
 * A363674  T(n,k) is the decimal equivalent of the n-bit inverted Gray code
 *          for k; triangle T(n,k), n>=0, 0<=k<=2^n-1, read by rows.
 * https://oeis.org/A363674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, look
 * Formula:   not stated
 * Range:     u8 n <= 510, u16 n <= 9831, u32 n <= 9831, u64 n <= 9831,
 *            u128 n <= 9831, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* 
 * Formula: a(n) = (2^r - 1) - (k ^ (k >> 1)),
 * where r = floor(log2(n+1)) and k = n - (2^r - 1).
 * This is the decimal value of the inverted n‑bit Gray code at position k.
 */
seq_t a(idx_t n) {
    /* Compute r = floor(log2(n+1)) */
    idx_t m = n + 1;           /* n+1 fits in idx_t (unsigned 64‑bit) */
    idx_t r = 0;
    while (m > 1) {
        m >>= 1;
        ++r;
    }

    /* 2^r - 1 */
    seq_t row_start = ((seq_t)1 << r) - 1;

    /* k = n - (2^r - 1) */
    seq_t k = (seq_t)n - row_start;

    /* Gray(k) = k ^ (k >> 1) */
    seq_t gray = k ^ (k >> 1);

    /* T(r,k) = (2^r - 1) - Gray(k) */
    seq_t max_val = ((seq_t)1 << r) - 1;
    return max_val - gray;
}
