/*
 * A303224  a(0)=0, a(1)=1; for n>1, a(n) = n*a(n-1) - 3*a(n-2).
 * https://oeis.org/A303224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1; for n>1, a(n) = n*a(n-1) - 3*a(n-2)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* seq_t and idx_t are provided by the build system:
   idx_t is unsigned (uint64_t);
   seq_t is an unsigned integer type (uint8_t..uint64_t, unsigned __int128,
   or an arbitrary‑precision integer class). */
/* The function computes the nth term exactly, using only integer arithmetic
   and works for any build (C11 or C++17). */
seq_t a(idx_t n)
{
    /* 0 and 1 are special cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Previous two values of the sequence */
    seq_t a_nm2 = (seq_t)0;  /* a(n-2) */
    seq_t a_nm1 = (seq_t)1;  /* a(n-1) */
    seq_t a_n   = 0;         /* a(n) will hold the current term */

    /* Iterate from 2 up to n */
    for (idx_t i = 2; i <= n; ++i) {
        a_n = (seq_t)i * a_nm1 - (seq_t)3 * a_nm2;
        a_nm2 = a_nm1;      /* shift for next iteration */
        a_nm1 = a_n;
    }
    return a_n;
}
