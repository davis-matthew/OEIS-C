/*
 * A301843  Number of 3Xn 0..1 arrays with every element equal to 0, 1 or 2
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A301843:
 * a(n) = 4*a(n‑1) + 3*a(n‑2) + 3*a(n‑3) + 2*a(n‑4)
 *       - 2*a(n‑5) + 2*a(n‑6) - 7*a(n‑7) - 3*a(n‑8)
 *       + 5*a(n‑9) - 4*a(n‑10) - 3*a(n‑11) + a(n‑12)
 *
 * Initial values for n = 1 … 14:
 * 4, 32, 139, 678, 3182, 15199, 72514, 346244,
 * 1653214, 7893443, 37688062, 179946317, 859175558, 4102237937
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial values (index 0 unused) */
    static const seq_t init_vals[15] = {
        0ULL,              /* unused */
        4ULL,              /* a(1) */
        32ULL,             /* a(2) */
        139ULL,            /* a(3) */
        678ULL,            /* a(4) */
        3182ULL,           /* a(5) */
        15199ULL,          /* a(6) */
        72514ULL,          /* a(7) */
        346244ULL,         /* a(8) */
        1653214ULL,        /* a(9) */
        7893443ULL,        /* a(10) */
        37688062ULL,       /* a(11) */
        179946317ULL,      /* a(12) */
        859175558ULL,      /* a(13) */
        4102237937ULL      /* a(14) */
    };

    if (n <= 14 && n >= 1)
        return init_vals[n];

    /* Use a sliding window of the last 12 terms.
     * arr[0] = a(i-1), arr[1] = a(i-2), … , arr[11] = a(i-12)   */
    seq_t arr[12];

    /* initialise the window for i = 15 (uses a(14)…a(3)) */
    arr[0] = init_vals[14];
    arr[1] = init_vals[13];
    arr[2] = init_vals[12];
    arr[3] = init_vals[11];
    arr[4] = init_vals[10];
    arr[5] = init_vals[9];
    arr[6] = init_vals[8];
    arr[7] = init_vals[7];
    arr[8] = init_vals[6];
    arr[9] = init_vals[5];
    arr[10] = init_vals[4];
    arr[11] = init_vals[3];

    for (idx_t i = 15; i <= n; ++i)
    {
        seq_t next;

        /* For built‑in unsigned types up to 64 bits use 128‑bit
         * intermediate arithmetic to avoid temporary overflow. */
        if (sizeof(seq_t) <= 8) {
            __int128_t pos = 0;
            pos += (__int128_t)4 * (uint64_t)arr[0];
            pos += (__int128_t)3 * (uint64_t)arr[1];
            pos += (__int128_t)3 * (uint64_t)arr[2];
            pos += (__int128_t)2 * (uint64_t)arr[3];
            pos += (__int128_t)2 * (uint64_t)arr[5];
            pos += (__int128_t)5 * (uint64_t)arr[8];
            pos += (__int128_t)1 * (uint64_t)arr[11];

            __int128_t neg = 0;
            neg += (__int128_t)2 * (uint64_t)arr[4];
            neg += (__int128_t)7 * (uint64_t)arr[6];
            neg += (__int128_t)3 * (uint64_t)arr[7];
            neg += (__int128_t)4 * (uint64_t)arr[9];
            neg += (__int128_t)3 * (uint64_t)arr[10];

            next = (seq_t)(pos - neg);
        } else {
            /* For arbitrary‑precision types (or very large built‑ins)
             * use the type’s own arithmetic operators. */
            seq_t pos = 0;
            pos += arr[0] * 4;
            pos += arr[1] * 3;
            pos += arr[2] * 3;
            pos += arr[3] * 2;
            pos += arr[5] * 2;
            pos += arr[8] * 5;
            pos += arr[11];

            seq_t neg = 0;
            neg += arr[4] * 2;
            neg += arr[6] * 7;
            neg += arr[7] * 3;
            neg += arr[9] * 4;
            neg += arr[10] * 3;

            next = pos - neg;
        }

        /* Shift the window for the next iteration */
        for (int k = 11; k > 0; --k)
            arr[k] = arr[k - 1];
        arr[0] = next;
    }

    return arr[0];
}
