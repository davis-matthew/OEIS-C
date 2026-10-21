/*
 * A257300  Number of Motzkin paths of length n with no peaks at level 2.
 * https://oeis.org/A257300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence * n*a(n) + (-5*n+3)*a(n-1)
 *            + 6*n*a(n-2) + 2*(n-9)*a(n-3) * + 6*(-n+4)*a(n-4) + (n-6)*a(n-5)
 *            + 3*(-n+3)*a(n-6) = 0 * Initial values: a(0)=1, a(1)=1, a(2)=2,
 *            a(3)=4, a(4)=8, a(5)=17, a(6)=38
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 25, u64 n <= 46, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const seq_t base[7] = {1ULL, 1ULL, 2ULL, 4ULL, 8ULL, 17ULL, 38ULL};

    if (n <= 6)
        return base[n];

#ifdef __cplusplus
    /* Arbitrary‑precision case – use seq_t operations directly. */
    seq_t a1 = base[1], a2 = base[2], a3 = base[3];
    seq_t a4 = base[4], a5 = base[5], a6 = base[6];

    for (idx_t i = 7; i <= n; ++i)
    {
        int64_t i64 = (int64_t)i;
        int64_t c0 = -5 * i64 + 3;
        int64_t c1 = 6 * i64;
        int64_t c2 = 2 * (i64 - 9);
        int64_t c3 = 6 * (-i64 + 4);
        int64_t c4 = i64 - 6;
        int64_t c5 = 3 * (-i64 + 3);

        seq_t sum = 0;
        sum += (c0 >= 0 ? a6 * (seq_t)c0 : -(a6 * (seq_t)(-c0)));
        sum += (c1 >= 0 ? a5 * (seq_t)c1 : -(a5 * (seq_t)(-c1)));
        sum += (c2 >= 0 ? a4 * (seq_t)c2 : -(a4 * (seq_t)(-c2)));
        sum += (c3 >= 0 ? a3 * (seq_t)c3 : -(a3 * (seq_t)(-c3)));
        sum += (c4 >= 0 ? a2 * (seq_t)c4 : -(a2 * (seq_t)(-c4)));
        sum += (c5 >= 0 ? a1 * (seq_t)c5 : -(a1 * (seq_t)(-c5)));

        seq_t ai = (-sum) / (seq_t)i;
        a1 = a2; a2 = a3; a3 = a4; a4 = a5; a5 = a6; a6 = ai;
    }
    return a6;
#else
    /* 64‑bit (or up to 128‑bit) unsigned types – use 128‑bit intermediate. */
    seq_t a1 = base[1], a2 = base[2], a3 = base[3];
    seq_t a4 = base[4], a5 = base[5], a6 = base[6];

    for (idx_t i = 7; i <= n; ++i)
    {
        int64_t i64 = (int64_t)i;
        int64_t c0 = -5 * i64 + 3;
        int64_t c1 = 6 * i64;
        int64_t c2 = 2 * (i64 - 9);
        int64_t c3 = 6 * (-i64 + 4);
        int64_t c4 = i64 - 6;
        int64_t c5 = 3 * (-i64 + 3);

        __int128 sum = 0;
        sum += (__int128)c0 * (__int128)a6;
        sum += (__int128)c1 * (__int128)a5;
        sum += (__int128)c2 * (__int128)a4;
        sum += (__int128)c3 * (__int128)a3;
        sum += (__int128)c4 * (__int128)a2;
        sum += (__int128)c5 * (__int128)a1;

        seq_t ai = (seq_t)(-sum / i64);
        a1 = a2; a2 = a3; a3 = a4; a4 = a5; a5 = a6; a6 = ai;
    }
    return a6;
#endif
}
