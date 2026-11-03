/*
 * A300571  a(n) is the concatenation n in base 2, n-1 in base 2, ..., 0 in
 *          base 2.
 * https://oeis.org/A300571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of binary(n), binary(n-1), …, binary(0)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Validated: standard, pass, 29 terms through n = 137 (2026-11-01)
 * Helpers:   ilog2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the decimal integer obtained by concatenating the binary
 * representations of all numbers from n down to 0.
 *
 * The function uses only integer arithmetic and the provided utility
 * functions pow10() and floor_log2_uint64().
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/ilog2.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t result = 0;

    for (idx_t i = n; ; --i) {
        /* Determine the length of the binary representation of i.
         * For i == 0 the binary representation is "0", which has length 1.
         */
        idx_t len = 1;
        if (i != 0) {
            len = ilog2(i) + 1;
        }

        /* Convert the binary representation of i into its decimal value.
         * For example, binary 101 -> decimal 101.
         */
        seq_t val = 0;
        for (idx_t bit = len; bit > 0; --bit) {
            /* Most significant bit first. */
            seq_t bitval = ( (i >> (bit - 1)) & 1ULL );
            val = val * 10 + bitval;
        }

        /* Append this decimal value to the running result. */
        result = result * pow10(len) + val;

        if (i == 0) break;   /* completed the sequence */
    }

    return result;
}
