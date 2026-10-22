/*
 * A319678  Numbers with property that the first digit is the length of the
 *          number (written in base 10).
 * https://oeis.org/A319678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, fini
 * Formula:   a(n) = d*10^(d-1) + (n - (1 + (10^(d-1)-1)/9)), where d = min{ k
 *            | n <= (10^k -1)/9 } (enumeration of numbers whose first digit
 *            equals their length)
 * Range:     u8 n <= 11, u16 n <= 1111, u32 n <= 1111, u64 n <= 1111,
 *            u128 n <= 1111, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;          /* promote index to seq_t */
    seq_t prefix = 0;                /* cumulative count of terms before current block */
    seq_t pow10_val = 1;             /* 10^(d-1) for current block (d starts at 1) */

    for (uint8_t d = 1; d <= 9; ++d) {
        seq_t block = pow10_val;                 /* number of terms in this block */
        seq_t new_sum = prefix + block;          /* cumulative count up to this block */

        if (n_seq <= new_sum) {
            seq_t start_index = prefix + 1;      /* first index of this block */
            seq_t offset = n_seq - start_index;  /* offset within the block 0‑based */
            return (seq_t)d * pow10_val + offset;/* compute the n-th term */
        }

        prefix = new_sum;                       /* advance to next block */
        pow10_val *= (seq_t)10;                 /* 10^(d) for next iteration */
    }
    /* n is larger than the last defined term (a(111111111) = 999999999) */
    return 0;
}
