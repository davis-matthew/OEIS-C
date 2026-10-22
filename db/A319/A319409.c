/*
 * A319409  a(n) = n - A318921(n).
 * https://oeis.org/A319409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n - A318921(n) where A318921(n) is obtained by shortening
 *            each run of equal consecutive bits in the binary representation
 *            of n by one bit
 * Range:     u8 n <= 255, u16 n <= 8264, u32 n <= 8264, u64 n <= 8264,
 *            u128 n <= 8264, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case: n = 0 */
    if (n == 0)
        return 0;

    /* Convert the index to the output type (may be arbitrary precision) */
    seq_t N = (seq_t)n;

    /* Determine the number of bits in n */
    idx_t bitlen = 0;
    idx_t temp = n;
    while (temp > 0) {
        temp >>= 1;
        ++bitlen;
    }

    /* Initialise variables for scanning the bits from most significant
       to least significant. */
    idx_t pos = bitlen - 1;                     /* position of the msb */
    unsigned curbit = (n >> pos) & 1u;          /* value of current run */
    idx_t run_len = 1;                          /* length of current run */

    seq_t shortened = 0;                        /* value of shortened number */

    /* Scan the remaining bits (excluding the msb which was used above). */
    while (pos-- > 0) {
        unsigned bit = (n >> pos) & 1u;
        if (bit == curbit) {
            ++run_len;        /* same run continues */
        } else {
            /* Output (run_len-1) copies of curbit. */
            idx_t bits = run_len - 1;
            shortened <<= bits;
            if (curbit) {
                seq_t mask = ((seq_t)1 << bits) - 1;
                shortened |= mask;
            }
            curbit = bit;
            run_len = 1;
        }
    }

    /* Process the final run. */
    idx_t bits = run_len - 1;
    shortened <<= bits;
    if (curbit) {
        seq_t mask = ((seq_t)1 << bits) - 1;
        shortened |= mask;
    }

    /* The sequence value is the difference N - shortened. */
    return N - shortened;
}
