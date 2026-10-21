/*
 * A290986  Expansion of x^6/((1 - x)^2*(1 - 2*x + x^3 - x^4)).
 * https://oeis.org/A290986
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 38, u64 n <= 74, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A290986  
   Formula:  a(n) = A049858(n-2) - (n-2)  
   Recurrence (valid for n ≥ 13):
       a(n) = 4*a(n-1) - 5*a(n-2) + a(n-3) + 3*a(n-4) - 3*a(n-5) + a(n-6)
   Initial values:
       a(6)=1, a(7)=4, a(8)=11, a(9)=25, a(10)=52, a(11)=103
   The routine works for any n ≥ 6; for n < 6 the value is 0 (offset not
   required in the OEIS entry).  It does not allocate any dynamic memory
   and thus compiles under both C11 and C++17. */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Base values a(6)..a(11) */
    const seq_t a6 = (seq_t)1;
    const seq_t a7 = (seq_t)4;
    const seq_t a8 = (seq_t)11;
    const seq_t a9 = (seq_t)25;
    const seq_t a10 = (seq_t)52;
    const seq_t a11 = (seq_t)103;

    /* n is unsigned; no overflow on < 6 comparison. */
    if (n < 6) {
        return (seq_t)0;
    }
    if (n == 6) return a6;
    if (n == 7) return a7;
    if (n == 8) return a8;
    if (n == 9) return a9;
    if (n == 10) return a10;
    if (n == 11) return a11;

    /* For n >= 12 we iterate from 12 up to n using the 6‑term recurrence. */
    seq_t a0 = a6;   /* a(n-6) */
    seq_t a1 = a7;   /* a(n-5) */
    seq_t a2 = a8;   /* a(n-4) */
    seq_t a3 = a9;   /* a(n-3) */
    seq_t a4 = a10;  /* a(n-2) */
    seq_t a5 = a11;  /* a(n-1) */

    /* Iterate from current index i = 12 up to n */
    for (idx_t i = 12; i <= n; ++i) {
        seq_t next =   (seq_t)4 * a5
                      - (seq_t)5 * a4
                      + a3
                      + (seq_t)3 * a2
                      - (seq_t)3 * a1
                      + a0;
        /* Shift the window: advance one step */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }

    /* After the loop, a5 holds a(n) */
    return a5;
}
