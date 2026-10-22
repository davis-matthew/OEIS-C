/*
 * A343730  a(n) is the length of the longest run of consecutive integers
 *          centered at n/2 with primality symmetric about n/2, or 0 if no
 *          such run exists.
 * https://oeis.org/A343730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the longest run of consecutive integers
 *            centered at n/2 whose primality is symmetric about n/2. Computed
 *            by outward expansion versus the symmetry condition
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* n >= 1 */
    seq_t len = 0;

    if (n & 1U) {           /* n is odd – center is a half‑integer, run length is even */
        idx_t low  = n / 2;          /* floor(n/2)   */
        idx_t high = low + 1;        /* ceil(n/2)    */

        bool p_low  = (low  > 1 && is_prime(low));
        bool p_high = (high > 1 && is_prime(high));

        if (p_low != p_high)
            return (seq_t)0;          /* no symmetric pair at the center */

        len = (seq_t)2;               /* first matching pair */
        int64_t d = 1;                /* distance from the central pair */

        for (;;) {
            int64_t l = (int64_t)low  - d;
            int64_t h = (int64_t)high + d;

            bool pl = (l > 1 && is_prime((idx_t)l));
            bool ph = (h > 1 && is_prime((idx_t)h));

            if (pl != ph)
                break;

            len += (seq_t)2;
            ++d;
        }
    } else {                     /* n is even – center is an integer, run length is odd */
        idx_t mid = n / 2;

        len = (seq_t)1;           /* the center element is always part of the run */

        int64_t k = 1;            /* distance from the center */

        for (;;) {
            int64_t low  = (int64_t)mid - k;
            int64_t high = (int64_t)mid + k;

            bool pl = (low  > 1 && is_prime((idx_t)low));
            bool ph = (high > 1 && is_prime((idx_t)high));

            if (pl != ph)
                break;

            len += (seq_t)2;
            ++k;
        }
    }

    return len;
}
