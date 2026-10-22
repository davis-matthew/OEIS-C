/*
 * A357180  First run-length of the n-th composition in standard order.
 * https://oeis.org/A357180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = first run‑length of the n-th composition in standard
 *            order
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    idx_t tmp = n;
    idx_t idx = 0;          /* current bit index (starting from 0) */
    idx_t prev = 0;         /* position (1‑based) of previous set bit   */
    int   diff_len = 0;     /* number of differences (i.e., parts)     */
    idx_t diff[65];         /* sufficient for a 64‑bit integer          */

    /* Build the list of differences based on set bits of n. */
    while (tmp) {
        if (tmp & 1ULL) {
            idx_t pos = idx + 1;           /* position in the reversed string (1‑based) */
            diff[diff_len++] = pos - prev; /* first–difference (part length)          */
            prev = pos;
        }
        idx++;
        tmp >>= 1;
    }

    /* Compute the first run‑length from the reversed difference list. */
    idx_t run_len = 1;                     /* at least one part exists */
    for (int i = diff_len - 2; i >= 0; --i) {
        if (diff[i] == diff[i + 1]) {
            run_len++;
        } else {
            break;
        }
    }

    return (seq_t)run_len;
}
