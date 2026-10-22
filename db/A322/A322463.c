/*
 * A322463  Reverse runs of zeros in binary expansion of n and convert back to
 *          decimal.
 * https://oeis.org/A322463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = number obtained by reversing runs of zeros in the binary
 *            representation of n and interpreting the result as a decimal
 *            integer
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special case for zero. */
    if (n == 0)
        return (seq_t)0;

    /* Compute the number of bits in the binary representation of n. */
    size_t bitlen = 0;
    idx_t tmp = n;
    while (tmp) {
        ++bitlen;
        tmp >>= 1;
    }

    /* Gather the lengths of all runs of consecutive zeros in the order they
       appear in the original binary string. */
    size_t runs[64];   /* enough for 64‑bit numbers */
    size_t runcnt = 0;
    bool in_zero = false;
    size_t cur = 0;
    for (int i = (int)bitlen - 1; i >= 0; --i) {
        if ((n >> i) & 1) {           /* bit is 1 */
            if (in_zero) {
                runs[runcnt++] = cur;
                cur = 0;
                in_zero = false;
            }
        } else {                       /* bit is 0 */
            if (!in_zero) {
                in_zero = true;
            }
            ++cur;
        }
    }
    if (in_zero) {
        runs[runcnt++] = cur;
    }

    /* Reconstruct the output binary string, replacing each original zero
       run by the corresponding reversed run length. */
    seq_t result = 0;
    int i = (int)bitlen - 1;
    size_t rev_idx = runcnt;          /* start from the end of runs array */
    while (i >= 0) {
        if ((n >> i) & 1) {           /* one bit: output as is */
            result = (result << 1) | (seq_t)1;
            --i;
        } else {                       /* start of a zero run */
            /* Skip the original zeros */
            while (i >= 0 && !((n >> i) & 1)) {
                --i;
            }
            /* Append the reversed run length in zeros */
            size_t rev_len = runs[--rev_idx];
            result <<= rev_len;
        }
    }
    return result;
}
