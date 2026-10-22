/*
 * A333767  Length of shortest run of zeros after a one in the binary
 *          expansion of n. a(0) = 0.
 * https://oeis.org/A333767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the shortest run of consecutive zeros *
 *            immediately following a 1 in the binary expansion of n. * a(0) =
 *            0. For n > 0 the value is defined by examining all '1' bits; *
 *            the minimal number of zeros before the next '1' (or the end of
 *            the * expansion) is returned. If no zeros occur (...
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <limits.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)0;

    int msb   = (int)floor_log2_uint64(n);
    int minlen = INT_MAX;     /* will hold the minimal zero run length */
    int zeros  = -1;          /* -1 means "not currently counting zeros" */

    for (int i = msb; i >= 0; --i) {
        if ((n >> i) & 1U) {            /* current bit is a 1 */
            if (zeros >= 0) {           /* a previous run ended here */
                if (zeros < minlen)     /* update minimal length */
                    minlen = zeros;
                if (zeros == 0)         /* consecutive ones found */
                    return (seq_t)0;
            }
            zeros = 0;                 /* start counting zeros after this 1 */
        } else {                         /* current bit is a 0 */
            if (zeros >= 0)             /* only count zeros after a 1 */
                zeros++;
        }
    }

    /* Handle run of zeros that continues to the end of the number */
    if (zeros >= 0 && zeros < minlen)
        minlen = zeros;

    /* If no zero run was found (e.g. number is all 1's), return 0 */
    if (minlen == INT_MAX)
        return (seq_t)0;

    return (seq_t)minlen;
}
