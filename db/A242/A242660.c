/*
 * A242660  Nonnegative numbers of the form x^2+xy-2y^2.
 * https://oeis.org/A242660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-4)-a(n-5) for n>5 * Initial values: a(1)=0,
 *            a(2)=1, a(3)=4, a(4)=7, a(5)=9 * OEIS A242660 – nonnegative
 *            numbers of the form x^2+xy-2y^2
 * Range:     u8 n <= 114, u16 n <= 9082, u32 n <= 9082, u64 n <= 9082,
 *            u128 n <= 9082, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out‑of‑offset behaviour */

    /* base cases */
    const seq_t a1 = 0;
    const seq_t a2 = 1;
    const seq_t a3 = 4;
    const seq_t a4 = 7;
    const seq_t a5 = 9;

    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;
    if (n == 4) return a4;
    if (n == 5) return a5;

    /* ring buffer holding the last five values:
       v[0] = a(i-5), v[1] = a(i-4), v[2] = a(i-3),
       v[3] = a(i-2), v[4] = a(i-1) at the start of each loop */
    seq_t v[5];
    v[0] = a1;
    v[1] = a2;
    v[2] = a3;
    v[3] = a4;
    v[4] = a5;

    seq_t cur = 0;  /* will hold a(i) during the loop */
    for (idx_t i = 6; i <= n; ++i) {
        cur = v[4] + v[1] - v[0];   /* a(i-1)+a(i-4)-a(i-5) */
        /* shift buffer: discard oldest, move others, store new */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = cur;
    }

    return cur;
}
