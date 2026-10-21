/*
 * A266445  Binary representation of the middle column of the "Rule 25"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1)+a(n-2)-10*a(n-3) for n >= 14 Initial values:
 *            a(0)=1, a(1)=10, a(2)=100, a(3)=1000, a(4)=10001, a(5)=100010,
 *            a(6)=1000101, a(7)=10001010, a(8)=100010101, a(9)=1000101011,
 *            a(10)=10001010110, a(11)=100010101100, a(12)=1000101011000,
 *            a(13)=10001010110001. This recurrence follows from...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* pre‑computed values for 0 <= n <= 13 */
    static const seq_t init[14] = {
        1ULL,            /* a(0) */
        10ULL,           /* a(1) */
        100ULL,          /* a(2) */
        1000ULL,         /* a(3) */
        10001ULL,        /* a(4) */
        100010ULL,       /* a(5) */
        1000101ULL,      /* a(6) */
        10001010ULL,     /* a(7) */
        100010101ULL,    /* a(8) */
        1000101011ULL,   /* a(9) */
        10001010110ULL,  /* a(10) */
        100010101100ULL, /* a(11) */
        1000101011000ULL,/* a(12) */
        10001010110001ULL/* a(13) */
    };

    if (n < 14)
        return init[n];

    /* use recurrence a(n) = 10*a(n-1)+a(n-2)-10*a(n-3) for n >= 14 */
    seq_t a_nm3 = init[11];  /* a(n-3), start with n = 14 => a(11) */
    seq_t a_nm2 = init[12];  /* a(n-2), start with a(12) */
    seq_t a_nm1 = init[13];  /* a(n-1), start with a(13) */

    for (idx_t i = 14; i <= n; ++i) {
        seq_t cur = mul_small(a_nm1, 10) + a_nm2 - mul_small(a_nm3, 10);
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }
    return a_nm1;
}
