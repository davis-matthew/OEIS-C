/*
 * A321320  Largest power of 2 obtainable by partitioning the binary
 *          representation of n into consecutive blocks and then summing.
 * https://oeis.org/A321320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest power of 2 obtainable by partitioning the *
 *            binary representation of n into consecutive blocks and summing *
 *            the numeric values of the blocks
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Test whether a seq_t value is a power of two. */
static inline bool is_power_of_two(seq_t v) {
    return v != 0 && ((v & (v - 1)) == 0);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* not part of the sequence, safety value */

    /* Determine bit length L of n */
    int L = 0;
    idx_t tmp = n;
    while (tmp) {
        ++L;
        tmp >>= 1;
    }

    /* Store bits of n from most significant to least significant */
    unsigned char bits[64];               /* 64 is safe for all idx_t sizes */
    tmp = n;
    for (int i = L - 1; i >= 0; --i) {
        bits[i] = tmp & 1u;
        tmp >>= 1;
    }

    /* Enumerate all 2^(L-1) partitions using a mask.
     * For L==1 the only mask is 0. */
    seq_t maxPower = 1;                   /* at least 1 for n >= 1 */
    idx_t partitions = ((idx_t)1) << (L - 1);

    for (idx_t mask = 0; mask < partitions; ++mask) {
        seq_t sum = 0;                    /* sum for this partition */
        seq_t current = 0;                /* current block value */

        for (int i = 0; i < L; ++i) {
            current = (current << 1) | bits[i];
            if (i == L - 1 || ((mask >> i) & 1)) {
                sum += current;
                current = 0;
            }
        }

        if (is_power_of_two(sum) && sum > maxPower) {
            maxPower = sum;
        }
    }

    return maxPower;
}
