/*
 * A386907  Number of spanning trees of the class H_3.
 * https://oeis.org/A386907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 12, u64 n <= 26, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A386907: Number of spanning trees of the class H_3
 * Recurrence: a(0)=75, a(1)=336, a(2)=1488, a(3)=6580, a(4)=29085
 * a(n) = 5*a(n-1) - 3*a(n-2) + 3*a(n-3) - 5*a(n-4) + a(n-5)   for n >= 5
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first five terms directly */
    switch (n) {
        case 0: return 75;
        case 1: return 336;
        case 2: return 1488;
        case 3: return 6580;
        case 4: return 29085;
    }

    /* a0 .. a4 will hold a(n-5) .. a(n-1) during the loop */
    seq_t a0 = 75;      /* a(0) */
    seq_t a1 = 336;     /* a(1) */
    seq_t a2 = 1488;    /* a(2) */
    seq_t a3 = 6580;    /* a(3) */
    seq_t a4 = 29085;   /* a(4) */
    seq_t next = 0;

    idx_t i;
    for (i = 5; i <= n; ++i) {
        /* Apply the recurrence */
        next = 5 * a4
               - 3 * a3
               + 3 * a2
               - 5 * a1
               + a0;

        /* Shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }

    return a4;  /* a(n) is now stored in a4 */
}
