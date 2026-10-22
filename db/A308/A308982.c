/*
 * A308982  Areas of integer rectangles circumscribed by increasingly large
 *          circles.
 * https://oeis.org/A308982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th area in the sorted list of all integer
 *            rectangles (a,b) with 1 ≤ a ≤ b, sorted first by the
 *            circumscribed circle radius (i.e. by s = a² + b², ascending) and
 *            ties broken by area a·b. Return the area as a seq_t
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Integer floor square root for unsigned 64‑bit values (binary search) */
static idx_t _isqrt_u64(idx_t n) {
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid > n / mid)
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;   /* unused for valid indices */

    idx_t count = 0;       /* how many areas built so far */
    idx_t s = 2;           /* smallest possible a² + b² with a,b ≥ 1 */

    /* temporary buffer for areas with the same radius s; a,b ≤ sqrt(s) */
    seq_t area_buf[256];
    size_t area_cnt;

    while (1) {
        area_cnt = 0;
        idx_t limit_a = _isqrt_u64(s / 2);   /* ensures a ≤ b */

        for (idx_t a_val = 1; a_val <= limit_a; ++a_val) {
            idx_t b2 = s - a_val * a_val;
            idx_t b_val = _isqrt_u64(b2);
            if (b_val * b_val == b2 && b_val >= a_val) {
                area_buf[area_cnt++] = (seq_t)a_val * (seq_t)b_val;
            }
        }

        /* sort the small array of areas for this radius s */
        for (size_t i = 1; i < area_cnt; ++i) {
            seq_t key = area_buf[i];
            size_t j = i;
            while (j > 0 && area_buf[j - 1] > key) {
                area_buf[j] = area_buf[j - 1];
                --j;
            }
            area_buf[j] = key;
        }

        /* output the sorted areas, count towards n */
        for (size_t i = 0; i < area_cnt; ++i) {
            ++count;
            if (count == n) return area_buf[i];
        }

        ++s;   /* next radius squared */
    }
}
