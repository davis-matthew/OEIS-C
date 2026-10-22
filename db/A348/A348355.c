/*
 * A348355  The base-7 expansion of a(n) is obtained by replacing 1's, 2's,
 *          3's, 4's, 5's and 6's by 4's, 5's, 6's, 1's, 2's and 3's,
 *          respectively, in the base-7 expansion of n.
 * https://oeis.org/A348355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the number obtained from the base‑7 representation of n
 *            by replacing each digit according to the involution 0→0, 1↔4,
 *            2↔5, 3↔6, and then reinterpreting the mapped digits as a base‑7
 *            number
 * Range:     u8 n <= 104, u16 n <= 2400, u32 n <= 2400, u64 n <= 2400,
 *            u128 n <= 2400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Special case for n == 0 */
    if (n == 0) {
        return 0;
    }

    /* Temporary storage for mapped digits (little‑endian order) */
    uint8_t digits[64];   /* enough for all n < 2^64 (max 20 base‑7 digits) */
    size_t len = 0;

    /* Convert n to base‑7, map each digit, and store the result */
    while (n > 0) {
        uint8_t d = (uint8_t)(n % 7);
        uint8_t m;
        if (d == 0) {
            m = 0;
        } else if (d < 4) {
            m = d + 3;   /* 1→4, 2→5, 3→6 */
        } else {
            m = d - 3;   /* 4→1, 5→2, 6→3 */
        }
        digits[len++] = m;
        n /= 7;
    }

    /* Reconstruct the number from the mapped digits (most‑significant first) */
    seq_t result = 0;
    while (len > 0) {
        result = result * 7 + digits[--len];
    }

    return result;
}
