/*
 * A281223  The decimal representation of the Elias omega code for n
 *          (A281193(n)).
 * https://oeis.org/A281223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the decimal value of the binary Elias omega code for n
 * Range:     u8 n <= 15, u16 n <= 511, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_log2_uint64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_log2_uint64.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Collect the binary pieces in a stack
       for the successive values of n during
       the recursive construction of omega.     */
    seq_t  val_stack[64];
    idx_t  len_stack[64];
    size_t pos = 0;

    /* For each step, store the current n
       and its bit-length (1 + floor_log2(n)).   */
    while (n > 1) {
        idx_t len = floor_log2_uint64(n) + 1;
        val_stack[pos] = (seq_t)n;
        len_stack[pos] = len;
        pos++;
        n = floor_log2_uint64(n);          /* next n = len-1 = floor_log2(n)  */
    }

    /* Reconstruct the final value by processing the
       stored pieces in reverse order (rightmost first),
       each time shifting left by its length and appending
       the current piece's integer value.           */
    seq_t value = 0;
    for (size_t i = pos; i-- > 0; ) {
        value = value * pow2(len_stack[i]) + val_stack[i];
    }

    /* Append the terminating 0 bit: just shift left by one. */
    value *= 2;

    return value;
}
