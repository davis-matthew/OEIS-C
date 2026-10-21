/*
 * A291160  Sequence of red numbers on the double zero (American) roulette
 *          wheel read counterclockwise.
 * https://oeis.org/A291160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A291160: red numbers on the double zero (American) roulette wheel read counterclockwise.
   The sequence is periodic with period 18, determined by the wheel layout:
   cycle = {14,23,16,21,18,19,12,25,27,1,36,3,34,5,32,7,30,9}   */
/* The function returns cycle[(n-1) modulo 18] for n >= 1. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* A period‑18 cycle derived from the roulette wheel layout. */
    static const seq_t cycle[18] = {
        14, 23, 16, 21, 18, 19,
        12, 25, 27,  1, 36,  3,
        34,  5, 32,  7, 30,  9
    };
    /* Compute zero‑based index of the term within the period. */
    idx_t pos = (n - 1) % 18;
    return cycle[(size_t)pos];
}
