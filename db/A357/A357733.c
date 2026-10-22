/*
 * A357733  Integer lengths of the sides of such regular hexagons that a
 *          polyline described in A356047 exists.
 * https://oeis.org/A357733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 208*a(n-2) – 2718*a(n-4) + 208*a(n-6) – a(n-8) for n > 8,
   with a(1)=1, a(2)=2, a(3)=286, a(4)=299, a(5)=56653, a(6)=56834,
   a(7)=11006686, a(8)=11009207 */

#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1 */

    static const seq_t base[9] = {
        0,                /* unused for a(0) */
        1, 2, 286, 299,   /* a(1)–a(4) */
        56653, 56834,     /* a(5)–a(6) */
        11006686, 11009207/* a(7)–a(8) */
    };

    if (n <= 8) return base[n];

    /* buffer last[0] = a(cur), last[1] = a(cur-1), … last[7] = a(cur-7) */
    seq_t last[8];
    last[0] = base[8];
    last[1] = base[7];
    last[2] = base[6];
    last[3] = base[5];
    last[4] = base[4];
    last[5] = base[3];
    last[6] = base[2];
    last[7] = base[1];

    for (idx_t cur = 9; cur <= n; ++cur) {
        seq_t new_val =
            mul_small(last[1], 208)          /* 208*a(cur-1) */
          - mul_small(last[3], 2718)        /* 2718*a(cur-3) */
          + mul_small(last[5], 208)          /* 208*a(cur-5) */
          - last[7];                        /*   a(cur-7)  */

        /* shift buffer: a(cur) becomes a(cur-1), … */
        for (int i = 7; i > 0; --i) last[i] = last[i-1];
        last[0] = new_val;
    }

    return last[0];
}
