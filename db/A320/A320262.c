/*
 * A320262  Write n in binary, then modify each run of 0's and each run of 1's
 *          by appending a 0. a(n) is the decimal equivalent of the result.
 * https://oeis.org/A320262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 297, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * nonn, base
 * Formula: a(n) = decimal value of the binary string obtained by
 *              writing n in binary and appending a 0 to each maximal run
 *              of equal bits.  Example: n = 6 (110) -> 11000 (24).
 * Recurrence (OEIS A320262):
 *   a(4n)   = 2*a(2n)
 *   a(4n+1) = 4*a(2n)+2
 *   a(4n+2) = 4*a(2n+1)
 *   a(4n+3) = 2*a(2n+1)+2
 */
#include <stdint.h>

/* Compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    seq_t res = 0;
    /* Determine length in bits of n (n > 0) */
    idx_t len = 0;
    for (idx_t temp = n; temp; temp >>= 1)
        ++len;

    int prev = -1;
    int runlen = 0;

    for (int i = (int)len - 1; i >= 0; --i) {
        int bit = (int)((n >> i) & 1);

        if (prev == -1) {
            prev = bit;
            runlen = 1;
            continue;
        }

        if (bit == prev) {
            ++runlen;
        } else {
            /* flush previous run */
            if (prev == 1) {
                seq_t ones = ((seq_t)1 << runlen) - 1;   /* runlen 1's */
                seq_t chunk = ones << 1;                 /* append 0 */
                res = (res << (runlen + 1)) | chunk;
            } else {
                /* run of zeros contributes only zeros */
                res <<= (runlen + 1);
            }
            prev = bit;
            runlen = 1;
        }
    }

    /* flush final run */
    if (prev == 1) {
        seq_t ones = ((seq_t)1 << runlen) - 1;
        seq_t chunk = ones << 1;
        res = (res << (runlen + 1)) | chunk;
    } else {
        res <<= (runlen + 1);
    }

    return res;
}
