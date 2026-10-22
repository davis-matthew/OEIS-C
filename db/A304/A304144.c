/*
 * A304144  Number of nX3 0..1 arrays with every element unequal to 1, 2, 3 or
 *          6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) + 11*a(n-3) - 5*a(n-4) - 39*a(n-5) * -
 *            29*a(n-6) + 4*a(n-7) + 42*a(n-8) + 15*a(n-9) + 12*a(n-10) * -
 *            2*a(n-11) - 4*a(n-12) - 16*a(n-13) - 8*a(n-14) * with initial
 *            values: * a(1)=1, a(2)=14, a(3)=18, a(4)=25, a(5)=113, a(6)=238,
 *            * a(7)=359, a(8)=1309, a(9)=3091, a(10)=5173...
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 51, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* initial 16 terms, a[0] is unused, a[1] … a[16] are defined */
    const seq_t init[17] = {
        (seq_t)0,  /* a(0), not used */
        (seq_t)1,  /* a(1) */
        (seq_t)14, (seq_t)18, (seq_t)25, (seq_t)113,
        (seq_t)238, (seq_t)359, (seq_t)1309, (seq_t)3091,
        (seq_t)5173, (seq_t)15475, (seq_t)39432, (seq_t)72084,
        (seq_t)189527, (seq_t)495712, (seq_t)981404
    };

    if (n <= 16)
        return init[(size_t)n];

    /* ring buffer of the last 14 values a(n-14)…a(n-1) */
    seq_t last[14];
    /* fill with a(3)…a(16) – the first 14 values needed for the recurrence */
    for (size_t i = 0; i < 14; ++i)
        last[i] = init[3 + i];

    /* iterate from a(17) up to the requested n */
    for (idx_t cur = 17; cur <= n; ++cur) {
        seq_t next = last[13];              /* a(n-1)   coefficient +1 */
        next += 3 * last[12];                 /* a(n-2)   coefficient +3 */
        next += 11 * last[11];                /* a(n-3)   coefficient +11 */
        next -= 5 * last[10];                 /* a(n-4)   coefficient -5 */
        next -= 39 * last[9];                 /* a(n-5)   coefficient -39 */
        next -= 29 * last[8];                 /* a(n-6)   coefficient -29 */
        next += 4 * last[7];                  /* a(n-7)   coefficient +4 */
        next += 42 * last[6];                 /* a(n-8)   coefficient +42 */
        next += 15 * last[5];                 /* a(n-9)   coefficient +15 */
        next += 12 * last[4];                 /* a(n-10)  coefficient +12 */
        next -= 2 * last[3];                  /* a(n-11)  coefficient -2 */
        next -= 4 * last[2];                  /* a(n-12)  coefficient -4 */
        next -= 16 * last[1];                 /* a(n-13)  coefficient -16 */
        next -= 8 * last[0];                  /* a(n-14)  coefficient -8 */

        /* shift the ring buffer: drop the oldest value */
        for (size_t i = 0; i < 13; ++i)
            last[i] = last[i + 1];
        last[13] = next;                      /* new a(n) becomes the newest */
    }

    return last[13];
}
