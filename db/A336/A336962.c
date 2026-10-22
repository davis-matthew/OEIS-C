/*
 * A336962  Right-rotate run-lengths of consecutive equal digits in binary
 *          representation of n.
 * https://oeis.org/A336962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = right‑rotate run‑lengths of consecutive equal bits * in
 *            the binary representation of n, then rebuild the binary number *
 *            starting from the same leading bit
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle n = 0 explicitly (binary representation "0"). */
    if (n == 0)
        return (seq_t)0;

    /* Compute the bit length of n (most significant 1). */
    idx_t temp = n;
    idx_t bitlen = 0;
    while (temp) {
        bitlen++;
        temp >>= 1;
    }

    /* Extract run‑lengths of consecutive bits.  A 64‑bit integer cannot
     * have more than 64 runs; declare a fixed array accordingly. */
    idx_t runs[64];
    idx_t r = 0;                         /* number of runs found */

    unsigned current_bit = (n >> (bitlen - 1)) & 1;
    idx_t run_len = 1;

    for (idx_t i = bitlen - 2; (idx_t)i < bitlen; --i) {  /* i is unsigned */
        unsigned bit = (n >> i) & 1;
        if (bit == current_bit) {
            run_len++;
        } else {
            runs[r++] = run_len;
            current_bit = bit;
            run_len = 1;
        }
    }
    runs[r++] = run_len;                 /* store the last run */

    /* Rotate the run‑lengths right by one position. */
    idx_t new_runs[64];
    if (r > 0) {
        new_runs[0] = runs[r - 1];
        for (idx_t i = 1; i < r; ++i)
            new_runs[i] = runs[i - 1];
    }

    /* Reconstruct the binary number using the rotated run‑lengths.
     * The starting bit is the same leading bit as in n. */
    seq_t result = 0;
    unsigned bitval = (n >> (bitlen - 1)) & 1;   /* leading bit */

    for (idx_t i = 0; i < r; ++i) {
        idx_t len = new_runs[i];
        for (idx_t j = 0; j < len; ++j) {
            result = (result << 1) | (seq_t)bitval;
        }
        bitval ^= 1;                          /* toggle after each run */
    }

    return result;
}
