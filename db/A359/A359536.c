/*
 * A359536  a(n) is the size of the largest subset of {0,1,...,n} such that
 *          the sum of two (not necessarily distinct) elements is never a
 *          power of 2.
 * https://oeis.org/A359536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 250, u16 n <= 250, u32 n <= 250, u64 n <= 250,
 *            u128 n <= 250, big any n
 * Helpers:   floor_log2_uint64, is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = 1                               for n <= 2
   a(n) = n/2                            for n a power of 2
   a(n) = (n - 2^k) + a(2^k - (n-2^k) - 1) for n not a power of 2
   where k = floor(log2(n)).
   This follows the OEIS definition: a(n) is the size of a
   maximal subset of {0,…,n} with no two elements summing to a power of 2.
   The function is recursive and runs in O(log n) time.
*/
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_power_of_two.h"
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n <= 2)                     /* a(0)=1, a(1)=1, a(2)=1 */
        return (seq_t)1;

    if (is_power_of_two(n))         /* a(2^k) = 2^(k-1) */
        return ((seq_t)n) >> 1;

    unsigned k = floor_log2_uint64(n);
    seq_t pow2k = ((seq_t)1ULL) << k;          /* 2^k  (fits in idx_t) */

    seq_t part = (seq_t)(n - pow2k);           /* n - 2^k */
    /* r = 2^k+1 - part ;  r-1 = pow2k - part - 1  */
    seq_t rminus1 = pow2k - part - 1ULL;

    return part + a((idx_t)rminus1);
}
