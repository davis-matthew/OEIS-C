/*
 * A279269  a(n) = floor( (4 + sqrt(11))^n ).
 * https://oeis.org/A279269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 8*a(n-1) - 5*a(n-2) + 2   for n > 1
 * Initial values: a(0) = 1, a(1) = 7
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the first two terms directly */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)7;

    /* a(0) and a(1) */
    seq_t a_nm2 = (seq_t)1;  /* a(n-2) */
    seq_t a_nm1 = (seq_t)7;  /* a(n-1) */
    seq_t a_val  = 0;        /* current a(n) */

    /* compute the sequence iteratively */
    for (idx_t i = 2; i <= n; ++i) {
        a_val = (seq_t)8 * a_nm1 - (seq_t)5 * a_nm2 + (seq_t)2;
        a_nm2 = a_nm1;
        a_nm1 = a_val;
    }

    return a_nm1;
}
