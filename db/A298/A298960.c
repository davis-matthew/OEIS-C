/*
 * A298960  Number of nX5 0..1 arrays with every element equal to 3, 5, 6, 7
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2) + 5*a(n-4) - 9*a(n-5) - a(n-6) * -
 *            8*a(n-7) + 8*a(n-9) - 6*a(n-10) + 2*a(n-11) * Initial values
 *            (1‑based): * a(1)=0, a(2)=3, a(3)=3, a(4)=14, a(5)=31, a(6)=91,
 *            * a(7)=230, a(8)=633, a(9)=1685, a(10)=4552, a(11)=12185
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the defined range explicitly to avoid wrap‑around on small types */
    static const seq_t init[12] = {
        0ULL,          /* dummy for index 0 */
        0ULL,          /* a(1) */
        3ULL,          /* a(2) */
        3ULL,          /* a(3) */
        14ULL,         /* a(4) */
        31ULL,         /* a(5) */
        91ULL,         /* a(6) */
        230ULL,        /* a(7) */
        633ULL,        /* a(8) */
        1685ULL,       /* a(9) */
        4552ULL,       /* a(10) */
        12185ULL       /* a(11) */
    };

    if (n <= 11)
        return init[n];

    /* Ring buffer holding the last 12 terms: a(k) where k % 12 == pos */
    seq_t ring[12];
    ring[0] = 0;                     /* unused until first computed value */
    for (int i = 1; i <= 11; ++i)
        ring[i] = init[i];

    for (idx_t i = 12; i <= n; ++i)
    {
        size_t i_mod  = (size_t)(i % 12);
        size_t p1     = (size_t)((i - 1) % 12);
        size_t p2     = (size_t)((i - 2) % 12);
        size_t i4     = (size_t)((i - 4) % 12);
        size_t i5     = (size_t)((i - 5) % 12);
        size_t i6     = (size_t)((i - 6) % 12);
        size_t i7     = (size_t)((i - 7) % 12);
        size_t i9     = (size_t)((i - 9) % 12);
        size_t i10    = (size_t)((i - 10) % 12);
        size_t i11    = (size_t)((i - 11) % 12);

        /* Compute using unsigned arithmetic (wrap‑around is defined) */
        seq_t res = 0;
        res += 3 * ring[p1];
        res -= ring[p2];
        res += 5 * ring[i4];
        res -= 9 * ring[i5];
        res -= ring[i6];
        res -= 8 * ring[i7];
        res += 8 * ring[i9];
        res -= 6 * ring[i10];
        res += 2 * ring[i11];

        ring[i_mod] = res;
    }

    return ring[(size_t)(n % 12)];
}
