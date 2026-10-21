/*
 * A274891  Number of nX4 0..2 arrays with no element equal to any value at
 *          offset (0,-1) (-1,-1) or (-2,0) and new values introduced in order
 *          0..2.
 * https://oeis.org/A274891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A274891:
   Number of n×4 0..2 arrays with constraints.
   Recurrence (valid for n > 18):
       a(n) = a(n‑1) + 8·a(n‑2) - 28·a(n‑4) - 9·a(n‑5)
              + 55·a(n‑6) + 16·a(n‑7) - 70·a(n‑8) - 9·a(n‑9)
              + 56·a(n‑10) - 28·a(n‑12) + a(n‑13) + 8·a(n‑14) - a(n‑16)
   Initial values a(1) … a(18) are taken from the OEIS b‑file.   */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial values (index 1 .. 18) */
    static const seq_t init[19] = {
        0ULL,                     /* unused 0th element */
        4ULL,   36ULL,   37ULL,   104ULL,   275ULL,   753ULL,   2049ULL,
        5602ULL, 15305ULL, 41866ULL, 114475ULL, 313122ULL, 856462ULL,
        2342559ULL, 6407780ULL, 17526908ULL, 47941766ULL, 131135884ULL
    };

    if (n <= 18)        /* look‑up for the first 18 terms */
        return init[n];

    /* Buffer holding the last 16 terms needed for the recurrence.
       buf[0] = a(i‑1), buf[1] = a(i‑2), … , buf[15] = a(i‑16).   */
    seq_t buf[17];
    for (int j = 0; j < 17; ++j)          /* initialise for i = 19   */
        buf[j] = init[18 - j];            /* buf[0] = a(18), … buf[15] = a(3) */

    seq_t a_i = 0;                         /* placeholder */
    for (idx_t i = 19; i <= n; ++i)
    {
        /* Positive contributions */
        seq_t sum_pos = buf[0]
                       + (seq_t)8  * buf[1]
                       + (seq_t)55 * buf[5]
                       + (seq_t)16 * buf[6]
                       + (seq_t)56 * buf[9]
                       + buf[12]
                       + (seq_t)8  * buf[13];

        /* Negative contributions */
        seq_t sum_neg = (seq_t)28 * buf[3]
                       + (seq_t)9  * buf[4]
                       + (seq_t)70 * buf[7]
                       + (seq_t)9  * buf[8]
                       + (seq_t)28 * buf[11]
                       + buf[15];

        a_i = sum_pos - sum_neg;          /* guaranteed to be non‑negative */

        /* Rotate buffer for next iteration */
        for (int j = 15; j >= 1; --j)
            buf[j] = buf[j - 1];
        buf[0] = a_i;
    }

    return a_i;                            /* value for the requested n */
}
