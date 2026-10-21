/*
 * A240929  Number of 10-digit positive integers in base n where the sum of
 *          the first k digits equals the sum of the last k digits.
 * https://oeis.org/A240929
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10*a(n-1)-45*a(n-2)+120*a(n-3)-210*a(n-4)+252*a(n-5)
 *            -210*a(n-6)+120*a(n-7)-45*a(n-8)+10*a(n-9)-a(n-10) for n>11
 *            Initial values: a(2)=126, a(3)=6046, a(4)=88428, a(5)=694360,
 *            a(6)=3705741, a(7)=15192604, a(8)=51418473, a(9)=150420187,
 *            a(10)=392406145, a(11)=933294637
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 13, u64 n <= 151,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[12] = {
    0ULL,                     /* a(0) unused */
    0ULL,                     /* a(1) unused */
    (seq_t)126ULL,            /* a(2) */
    (seq_t)6046ULL,           /* a(3) */
    (seq_t)88428ULL,          /* a(4) */
    (seq_t)694360ULL,         /* a(5) */
    (seq_t)3705741ULL,        /* a(6) */
    (seq_t)15192604ULL,       /* a(7) */
    (seq_t)51418473ULL,       /* a(8) */
    (seq_t)150420187ULL,      /* a(9) */
    (seq_t)392406145ULL,      /* a(10) */
    (seq_t)933294637ULL       /* a(11) */
};

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    if (n <= 11) return init[(size_t)n];

    /* allocate array for 0 .. n  (n fits in size_t for allocation) */
    size_t cap = (size_t)(n + 1);
    seq_t *v = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!v) abort();

    v[0] = v[1] = 0;
    for (idx_t i = 2; i <= 11; ++i) v[i] = init[i];

    /* recurrence for i = 12 .. n */
    for (idx_t i = 12; i <= n; ++i) {
        if (sizeof(seq_t) <= 8) {          /* use __int128 for safety */
            __int128 sumplus  = (__int128)v[i-1] * 10;
            sumplus += (__int128)v[i-3] * 120;
            sumplus += (__int128)v[i-5] * 252;
            sumplus += (__int128)v[i-7] * 120;
            sumplus += (__int128)v[i-9] * 10;

            __int128 summinus = (__int128)v[i-2] * 45;
            summinus += (__int128)v[i-4] * 210;
            summinus += (__int128)v[i-6] * 210;
            summinus += (__int128)v[i-8] * 45;
            summinus += (__int128)v[i-10] * 1;

            v[i] = (seq_t)(sumplus - summinus);
        } else {                           /* arbitrary‑precision : use normal operators */
            seq_t sumplus = 0, summinus = 0;
            sumplus += 10 * v[i-1];
            sumplus += 120 * v[i-3];
            sumplus += 252 * v[i-5];
            sumplus += 120 * v[i-7];
            sumplus += 10 * v[i-9];

            summinus += 45 * v[i-2];
            summinus += 210 * v[i-4];
            summinus += 210 * v[i-6];
            summinus += 45 * v[i-8];
            summinus += v[i-10];

            v[i] = sumplus - summinus;
        }
    }

    seq_t result = v[n];
    free(v);
    return result;
}
