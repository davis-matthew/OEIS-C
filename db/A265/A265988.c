/*
 * A265988  Number of n X 4 integer arrays with each element equal to the
 *          number of horizontal and antidiagonal neighbors not equal to
 *          itself.
 * https://oeis.org/A265988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 33, u64 n <= 63, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A265988
   Empirical recurrence:
       a(n) = a(n-1) + 2*a(n-2) + a(n-3)
              - a(n-5) - 3*a(n-6) + a(n-7)
   for n ≥ 8
   Initial values:
       a(1)=1, a(2)=2, a(3)=2, a(4)=2, a(5)=7, a(6)=7, a(7)=21
*/

#include <stdint.h>   /* uint64_t */
#include <stddef.h>   /* size_t */

/* --------------------------------------------------------------- */
/*  seq_t and idx_t are supplied by the build environment.        */
/*  The function below uses only arithmetic operators and loops   */
/*  that are valid in both C11 and C++17. No dynamic allocation    */
/*  is performed, so it works for arbitrary‑precision seq_t as    */
/*  well.                                                          */
/* --------------------------------------------------------------- */

static const seq_t base[8] = {
    0ULL, /* unused, a(0) not defined */
    1ULL, /* a(1) */
    2ULL, /* a(2) */
    2ULL, /* a(3) */
    2ULL, /* a(4) */
    7ULL, /* a(5) */
    7ULL, /* a(6) */
    21ULL /* a(7) */
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below offset */
    if (n <= 7) return base[n];    /* return known initial values */

    /*  Keep the last seven terms:
        t[0] = a(i-6), t[1] = a(i-5), t[2] = a(i-4),
        t[3] = a(i-3), t[4] = a(i-2), t[5] = a(i-1), t[6] = a(i)
     */
    seq_t t[7];
    t[0] = base[1];
    t[1] = base[2];
    t[2] = base[3];
    t[3] = base[4];
    t[4] = base[5];
    t[5] = base[6];
    t[6] = base[7];

    for (idx_t i = 8; i <= n; ++i) {
        seq_t new_val = t[6] + 2 * t[5] + t[4] - t[2] - 3 * t[1] + t[0];
        /* shift the window left by one position */
        t[0] = t[1];
        t[1] = t[2];
        t[2] = t[3];
        t[3] = t[4];
        t[4] = t[5];
        t[5] = t[6];
        t[6] = new_val;
    }

    return t[6];
}
