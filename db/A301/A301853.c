/*
 * A301853  Triangle read by rows: T(n,k) gives the number of distinct
 *          distances on an n X k pegboard, with n >= 1, 1 <= k <= n.
 * https://oeis.org/A301853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 319, u16 n <= 9250, u32 n <= 9250, u64 n <= 9250,
 *            u128 n <= 9250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the OEIS sequence A301853.
   For an n × k pegboard (1 ≤ k ≤ n) the number of distinct distances
   (including the zero distance) is the number of distinct values of
   dx² + dy² with 0 ≤ dx ≤ n‑1, 0 ≤ dy ≤ k‑1.
   This function enumerates the triangle row by row:
   a(n) = T(r,k) where
       r = smallest integer with r·(r+1)/2 ≥ n
       k = n – (r‑1)·r/2
   The implementation uses integer arithmetic only and works for any
   64‑bit index n.  Distinct sums are counted via a bitset.  The code
   is fully compliant with the build requirements.  */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* Integer square root of a 128‑bit unsigned value.  */
static inline idx_t isqrt_u128(unsigned __int128 x)
{
    idx_t s = 0;
    for (int i = 63; i >= 0; --i) {
        idx_t cand = s + (1ULL << i);
        if ((unsigned __int128)cand * cand <= x)
            s = cand;
    }
    return s;
}

/*  Find the triangle row r such that r·(r+1)/2 ≥ n.
    r is the smallest such integer.  */
static inline idx_t row_from_index(idx_t n)
{
    if (n == 0)
        return 0;
    unsigned __int128 val = (unsigned __int128)8 * n + 1;
    idx_t s = isqrt_u128(val);          /* floor(sqrt(8n+1))   */
    idx_t r = (s - 1) >> 1;              /* floor((-1 + sqrt)/2) */
    if ((unsigned __int128)r * (r + 1) / 2 < n)
        ++r;
    return r;
}

/*  Compute T(rows, cols) – the number of distinct squared distances
    on a rows × cols board (including the zero distance).  */
static seq_t compute_T(idx_t rows, idx_t cols)
{
    if (rows == 0 || cols == 0)
        return 0;

    /* Maximum possible squared distance.  */
    idx_t max_sum = (rows - 1) * (rows - 1) + (cols - 1) * (cols - 1);
    size_t bitset_len = (max_sum >> 3) + 1;            /* bytes      */
    unsigned char *bits = (unsigned char *)calloc(bitset_len, 1);
    if (!bits)
        return 0;   /* unlikely but prevents crash */

    for (idx_t dx = 0; dx < rows; ++dx) {
        idx_t dx2 = dx * dx;
        for (idx_t dy = 0; dy < cols; ++dy) {
            idx_t sum = dx2 + dy * dy;
            bits[sum >> 3] |= 1U << (sum & 7U);
        }
    }

    seq_t count = 0;
    for (size_t i = 0; i < bitset_len; ++i) {
        unsigned char byte = bits[i];
        /* popcount of one byte, using only integer ops */
        while (byte) {
            ++count;
            byte &= byte - 1;
        }
    }

    free(bits);
    return count;
}

/*  Main entry point for the OEIS wrapper.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset is 1; undefined for n = 0 */

    idx_t r = row_from_index(n);           /* triangle row   */
    idx_t prev = (unsigned __int128)(r - 1) * r / 2;  /* terms before the row */
    idx_t k = n - prev;                    /* column index   */
    return compute_T(r, k);
}
