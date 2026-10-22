/*
 * A316841  Three-column table read by rows giving integer sides of proper
 *          triangles (i,j,k) with i >= j >= k >= 1, j+k > i.
 * https://oeis.org/A316841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   a(n) enumerates all integer triangles (i,j,k) with * i >= j >= k
 *            >= 1 and j + k > i, ordered by increasing i, * then increasing
 *            j, then increasing k. * The sequence outputs i,j,k row‑wise
 * Range:     u8 n <= 9000, u16 n <= 9000, u32 n <= 9000, u64 n <= 9000,
 *            u128 n <= 9000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */
    idx_t pos = n - 1;                    /* zero‑based position in the sequence */
    idx_t tri_no = pos / 3;               /* zero‑based triangle number */
    idx_t offset = pos % 3;               /* 0 → i, 1 → j, 2 → k */

    /* Find the triangle with largest side i containing tri_no */
    idx_t i = 1;
    while (1) {
        /* Count how many triangles have largest side exactly i */
        idx_t j_start = (i + 1) / 2;
        idx_t cnt_tri_i = 0;

        for (idx_t j = j_start; j <= i; ++j) {
            idx_t k_start = (i > j) ? (i - j + 1) : 1;
            if (k_start <= j) {
                cnt_tri_i += (j - k_start + 1);
            }
        }

        if (tri_no < cnt_tri_i) {
            break;              /* the desired triangle is in this block */
        }
        tri_no -= cnt_tri_i;    /* skip all triangles with smaller i */
        ++i;
    }

    /* Find the j and k within the block of triangles with this i */
    idx_t j_start = (i + 1) / 2;
    idx_t j = j_start;
    while (1) {
        idx_t k_start = (i > j) ? (i - j + 1) : 1;
        idx_t cnt_k = 0;
        if (k_start <= j) {
            cnt_k = (j - k_start + 1);
        }

        if (tri_no < cnt_k) {
            break;              /* the desired triangle is in this j */
        }
        tri_no -= cnt_k;        /* skip all triangles with smaller j */
        ++j;
    }

    /* k is determined by the remaining offset within this j */
    idx_t k_start = (i > j) ? (i - j + 1) : 1;
    idx_t k = k_start + tri_no;

    /* Return the component requested by offset */
    if (offset == 0) return (seq_t)i;
    if (offset == 1) return (seq_t)j;
    return (seq_t)k;
}
