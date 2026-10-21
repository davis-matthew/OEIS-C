/*
 * A278965  Numbers k such that k! = 2^a * 3^b * c, where a and b are 0 or
 *          powers of 2 and c is relatively prime to 6.
 * https://oeis.org/A278965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   is_pow2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A278965
 * Numbers k such that k! = 2^a * 3^b * c, where a and b are 0 or powers of 2
 * and c is coprime to 6.
 *
 * This implementation searches k from 1 upwards, maintaining the cumulative
 * exponents of 2 and 3 in k! incrementally.  It returns the n‑th such k
 * (n ≥ 1).  No memoisation or dynamic arrays are used, so it works with
 * both integral seq_t types and an arbitrary‑precision seq_t class.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_pow2_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; callers will not pass 0 */
        return (seq_t)0;

    idx_t k = 1;                 /* current candidate for k */
    uint64_t expo2 = 0, expo3 = 0; /* cumulative exponents in k! */
    idx_t count = 0;              /* how many qualifying k's seen so far */

    while (true) {
        /* Add exponent of 2 in k to expo2 */
        idx_t t = k;
        uint64_t add2 = 0;
        while ((t & 1) == 0 && t != 0) {
            ++add2;
            t >>= 1;
        }
        expo2 += add2;

        /* Add exponent of 3 in k to expo3 */
        t = k;
        uint64_t add3 = 0;
        while (t % 3 == 0 && t != 0) {
            ++add3;
            t /= 3;
        }
        expo3 += add3;

        /* If both exponents are 0 or powers of two, k qualifies */
        if (is_pow2_uint64(expo2) && is_pow2_uint64(expo3)) {
            ++count;
            if (count == n)
                return (seq_t)k;     /* found the n‑th term */
        }

        ++k;
    }
}
