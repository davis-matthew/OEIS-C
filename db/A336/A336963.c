/*
 * A336963  Left-rotate run-lengths of consecutive equal digits in binary
 *          representation of n.
 * https://oeis.org/A336963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the definition of A336963:
 *   a(n) = binary formed by left‑rotating the run‑lengths of consecutive
 *          equal bits in the binary representation of n, and then
 *          interpreting those run‑lengths as alternating runs of 1’s and 0’s
 *          starting with 1.  a(0) = 0.
 */
#include <stdint.h>

/* Helper: return the number of bits needed to represent n (n>0). */
static int bitlen_uint64(idx_t n)
{
    int len = 0;
    while (n != 0) {
        ++len;
        n >>= 1;
    }
    return len;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* bin(0) = "0", a(0) = 0 */
        return 0;

    /* Determine bit length of n (no leading zeros). */
    int bitlen = bitlen_uint64(n);

    /* Collect run‑lengths of consecutive equal bits, from MSB to LSB. */
    unsigned char run[64];          /* maximum 64 runs for 64‑bit indices */
    int runcnt = 0;
    int prev = -1;                  /* none seen yet */
    int cur_len = 0;

    for (int i = bitlen - 1; i >= 0; --i) {
        int bit = (n >> i) & 1;
        if (bit == prev) {
            ++cur_len;              /* same run continues */
        } else {
            if (prev != -1) {
                run[runcnt++] = (unsigned char)cur_len;  /* store finished run */
            }
            cur_len = 1;              /* start new run */
            prev = bit;
        }
    }
    /* Store the final run */
    run[runcnt++] = (unsigned char)cur_len;

    /* Build the result by left‑rotating the run‑lengths and interpreting
     * them as alternating runs of 1’s and 0’s, starting with 1. */
    seq_t res = 0;
    int cur_bit = 1;                /* start with bit 1 */

    for (int j = 0; j < runcnt; ++j) {
        int len = run[(j + 1) % runcnt];   /* left‑rotate by 1 */
        for (int k = 0; k < len; ++k) {
            res = (res << 1) | (seq_t)cur_bit;
        }
        cur_bit ^= 1;   /* toggle to the other bit for the next run */
    }

    return res;
}
