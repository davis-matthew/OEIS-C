/*
 * A269071  Number of n X 4 binary arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling two not more than once.
 * https://oeis.org/A269071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) - 40*a(n-2) + 8*a(n-3) + 92*a(n-4) - 32*a(n-5)
 *            - 64*a(n-6) for n>7. Initial values: a(1)=13 a(2)=76 a(3)=521
 *            a(4)=3288 a(5)=20400 a(6)=123976 a(7)=742688
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* For n <= 7 just return the known values. */
    static const seq_t init[8] = {0,
        13ULL,          /* a(1) */
        76ULL,          /* a(2) */
        521ULL,         /* a(3) */
        3288ULL,        /* a(4) */
        20400ULL,       /* a(5) */
        123976ULL,      /* a(6) */
        742688ULL       /* a(7) */
    };

    if (n <= 7) {
        return init[(size_t)n];
    }

    /* Initialize the last six values as:
         a2 = a(n-6)
         a3 = a(n-5)
         a4 = a(n-4)
         a5 = a(n-3)
         a6 = a(n-2)
         a7 = a(n-1)
       Starting from n = 8, so a7 = a(7), a6 = a(6), … a2 = a(2).  */
    seq_t a2 = init[2];
    seq_t a3 = init[3];
    seq_t a4 = init[4];
    seq_t a5 = init[5];
    seq_t a6 = init[6];
    seq_t a7 = init[7];

    /* Compute terms up to n. */
    for (idx_t k = 8; k <= n; ++k) {
        /* Use only positive multiplications and a single subtraction. */
        seq_t pos = 12 * a7 + 8 * a5 + 92 * a4;
        seq_t neg = 40 * a6 + 32 * a3 + 64 * a2;
        seq_t term = pos - neg;               /* guaranteed non‑negative */

        /* Shift the window for the next iteration. */
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = a7;
        a7 = term;
    }

    return a7;
}
