/*
 * A319573  The y coordinates of the stripe enumeration of N X N where N = {0,
 *          1, 2, ...}.
 * https://oeis.org/A319573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = k if s is even * a(n) = s - k if s is odd * * where s =
 *            floor( (sqrt(8n+1) - 1) / 2 ) * k = n - s*(s+1)/2 (offset inside
 *            diagonal s) * * This is the y‑coordinate of the stripe
 *            enumeration of an N×N grid * where all non‑negative integer
 *            lattice points are listed by * increasing sum i+j, a...
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the largest s with s*(s+1)/2 <= n.  Linear search is O(sqrt(n))
 * which is well below the time limits for the required n. */

static inline idx_t diag_index(idx_t n) {
    idx_t s = 0;
    while (1) {
        idx_t nxt = s + 1;
        unsigned __int128 tri = (unsigned __int128)nxt * (nxt + 1) / 2;
        if (tri > n) break;
        s = nxt;
    }
    return s;
}

seq_t a(idx_t n) {
    idx_t s   = diag_index(n);
    unsigned __int128 prev = (unsigned __int128)s * (s + 1) / 2;
    idx_t k   = n - (idx_t)prev;          /* offset inside diagonal s */

    if ((s & 1) == 0)           /* even s: j increases with k */
        return (seq_t)k;
    else                         /* odd s:  j decreases with k */
        return (seq_t)(s - k);
}
