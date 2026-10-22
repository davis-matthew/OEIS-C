/*
 * A300410  Number of centered square numbers dividing n.
 * https://oeis.org/A300410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ k ≥ 0 : 2*k*(k+1)+1 divides n }
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/*  (centered square numbers are 1,5,13,25,41,… = 2k(k+1)+1) */
seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n == 0) return 0;          /* offset is 1, safety guard */

    for (idx_t k = 0; ; ++k)
    {
        /* compute centered square number 2*k*(k+1)+1 safely in 128‑bit */
        unsigned __int128 c128 = (unsigned __int128)2 * k * (k + 1) + 1;
        if (c128 > n) break;       /* no larger centered squares divide n */

        idx_t c = (idx_t)c128;     /* value fits in idx_t as c128 ≤ n ≤ 2^64−1 */

        if (n % c == 0) ++count;   /* count it as a divisor */
    }
    return count;
}
