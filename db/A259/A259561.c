/*
 * A259561  Numbers k such that k^2+2 is the product of a Fibonacci number and
 *          a Lucas number.
 * https://oeis.org/A259561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 28, u64 n <= 51, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1)=0, a(2)=1, a(3)=2, a(4)=3, a(5)=4, a(6)=10, a(7)=11,
   a(8)=29, a(9)=40, a(10)=76, a(11)=199, a(12)=521;
   for n>11:  a(n) = 3*a(n-1) – a(n-2). */

#include <stddef.h>

static const seq_t init_vals[13] = {
    0ULL,     /* dummy index 0, unused */
    0ULL,     /* a(1) */
    1ULL,     /* a(2) */
    2ULL,     /* a(3) */
    3ULL,     /* a(4) */
    4ULL,     /* a(5) */
    10ULL,    /* a(6) */
    11ULL,    /* a(7) */
    29ULL,    /* a(8) */
    40ULL,    /* a(9) */
    76ULL,    /* a(10) */
    199ULL,   /* a(11) */
    521ULL    /* a(12) */
};

seq_t a(idx_t n)
{
    if (n <= 12) {
        return init_vals[n];
    }

    /* start from the two last known terms */
    seq_t a_nm2 = init_vals[11];   /* a(11) */
    seq_t a_nm1 = init_vals[12];   /* a(12) */
    seq_t cur   = 0;

    for (idx_t i = 13; i <= n; ++i) {
        cur = (seq_t)3 * a_nm1 - a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }

    return a_nm1;
}
