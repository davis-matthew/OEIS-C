/*
 * A272881  Discriminator of the evil numbers (A001969).
 * https://oeis.org/A272881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   floor_log2_uint64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Discriminator of the evil numbers (A001969)  
   a(n) = 2^(i+1)-3                    if n = 2^i+1 with odd i > 2  
   a(n) = 2^(i+1)-1                    if n = 2^i+1 with even i ≥ 2  
   a(n) = 2^ceil(log2(n))              otherwise
   Uses helper functions floor_log2_uint64 and pow2. */

#include "../utils/floor_log2_uint64.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Check whether n = 2^i + 1 for some i */
    idx_t m = n - 1;                    /* m = n-1 */
    if (m && ((m & (m - 1)) == 0)) {    /* m is a power of two */
        idx_t i = floor_log2_uint64(m); /* i = log2(m) */
        if (i > 2 && (i & 1)) {         /* odd i > 2 */
            return pow2(i + 1) - 3;
        }
        if (i >= 2 && !(i & 1)) {       /* even i ≥ 2 */
            return pow2(i + 1) - 1;
        }
        /* For i = 0 or 1 the special rule does not apply */
    }

    /* Default: 2^ceil(log2(n)) */
    idx_t fl = floor_log2_uint64(n);          /* floor(log2(n)) */
    idx_t ceil = ((idx_t)1 << fl) == n ? fl : fl + 1;
    return pow2(ceil);
}
