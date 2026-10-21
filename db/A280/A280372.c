/*
 * A280372  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 260", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A280372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4) for n >= 13, with a(0)=1, a(1)=1, a(2)=101,
 *            a(3)=1, a(4)=1, a(5)=101, a(6)=10101, a(7)=1000001,
 *            a(8)=100000001, a(9)=101, a(10)=10101, a(11)=1000001, a(12)=1
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 126, u64 n <= 126, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    static const seq_t init[13] = {
        1ULL,                /* a(0) */
        1ULL,                /* a(1) */
        101ULL,              /* a(2) */
        1ULL,                /* a(3) */
        1ULL,                /* a(4) */
        101ULL,              /* a(5) */
        10101ULL,            /* a(6) */
        1000001ULL,          /* a(7) */
        100000001ULL,        /* a(8) */
        101ULL,              /* a(9) */
        10101ULL,            /* a(10) */
        1000001ULL,          /* a(11) */
        1ULL                 /* a(12) */
    };

    if (n < 13) {
        return init[(size_t)n];
    }

    /* For n >= 13, the sequence repeats the values of a(9) … a(12) every
       4 steps: a(n) = a(n-4). */
    idx_t mod = (n - 9) % 4;
    return init[9 + mod];
}
