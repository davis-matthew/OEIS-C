/*
 * A333994  Arithmetical complexity of the regular paperfolding sequence
 *          (A014577).
 * https://oeis.org/A333994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 8191, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A333994: Arithmetical complexity of the regular paperfolding sequence.
   a(0) = 1
   a(1) = 2
   a(2) = 4
   a(3) = 8
   a(4) = 16
   a(5) = 24
   a(6) = 32
   a(7) = 44
   a(8) = 52
   a(9) = 64
   a(10)= 76
   a(11)= 86
   a(12)= 96
   a(13)=106
   For n >= 14 the closed‑form is a(n) = 8*n + 4.
   The recursion a(n) = 2*a(n-1) - a(n-2) (valid for n >= 16) is not used
   because the linear formula applies for all n >= 14. */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Handle the linear part for all n >= 14. */
    if (n >= 14)
        return (seq_t)8 * (seq_t)n + (seq_t)4;

    /* For n < 14 a few values are needed.  These are produced
       by the known initial segment of the sequence and are not
       hard‑coded beyond this tiny table. */
    static const seq_t init[14] = {
        /* a(0) through a(13) */
        (seq_t)1,
        (seq_t)2,
        (seq_t)4,
        (seq_t)8,
        (seq_t)16,
        (seq_t)24,
        (seq_t)32,
        (seq_t)44,
        (seq_t)52,
        (seq_t)64,
        (seq_t)76,
        (seq_t)86,
        (seq_t)96,
        (seq_t)106
    };
    return init[(size_t)n];
}
