/*
 * A358135  Difference of first and last parts of the n-th composition in
 *          standard order.
 * https://oeis.org/A358135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A001511(n) - A065120(n)
 * Range:     i8 n <= 77, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* offset starts at 1, this case is unused */

    /* compute the sum m of the composition: 2^(m-1)-1 < n <= 2^m-1 */
    uint64_t m = 1 + (63 - __builtin_clzll(n));

    /* offset of n inside the compositions of sum m */
    uint64_t prev   = (m == 1) ? 0 : ((1ULL << (m - 1)) - 1);
    uint64_t offset = n - prev;          /* 1 .. 2^(m-1) */
    uint64_t k      = offset - 1;        /* 0 .. 2^(m-1)-1 */

    if (k == 0)                       /* no separator: only one part */
    {
        seq_t v = (seq_t)m;
        return v - v;                 /* difference is 0 */
    }

    /* position of the first and last separator (0‑based from the left of the gaps) */
    unsigned lsb = __builtin_ctzll(k);                     /* first separator */
    unsigned msb = 63 - __builtin_clzll(k);                /* last separator */

    seq_t first_part = (seq_t)(lsb + 1);
    seq_t last_part  = (seq_t)(m - (msb + 1));

    return first_part - last_part;
}
