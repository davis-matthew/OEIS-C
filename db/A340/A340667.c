/*
 * A340667  a(n) is derived from n by replacing each 0 in its binary
 *          representation with a string of n 0's.
 * https://oeis.org/A340667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is obtained by expanding the binary representation of n:
 * each 0 bit is replaced by a string of n consecutive 0's.
 */
#include <stdint.h>    /* for uintptr_t if needed */

seq_t a(idx_t n) {
    /* Handle the special case n = 0 explicitly */
    if (n == 0)
        return 0;

    /* Store the bits of n (least significant first). */
    int bits[64];          /* maximum possible bit length for a 64‑bit idx_t */
    int bitcnt = 0;
    idx_t temp = n;
    do {
        bits[bitcnt++] = (int)(temp & 1);
        temp >>= 1;
    } while (temp);

    seq_t result = 0;
    /* Process bits from most significant to least significant. */
    for (int i = bitcnt - 1; i >= 0; --i) {
        if (bits[i]) {                     /* bit is 1: append a single 1 */
            result = (result << 1) | 1;
        } else {                            /* bit is 0: append n zeros */
            for (idx_t k = 0; k < n; ++k)
                result <<= 1;
        }
    }
    return result;
}
