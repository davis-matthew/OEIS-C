/*
 * A289188  Eternal domination number for P_3 X P_n grid graph.
 * https://oeis.org/A289188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-5) + 4 for n > 26, with the initial values a(1)=2,
 *            a(2)=2, a(3)=3, a(4)=4, a(5)=5, a(6)=6, a(7)=7, a(8)=8, a(9)=8,
 *            a(10)=9, a(11)=10, a(12)=11, a(13)=12, a(14)=13, a(15)=14,
 *            a(16)=14, a(17)=15, a(18)=16, a(19)=17, a(20)=18, a(21)=18,
 *            a(22)=19, a(23)=20, a(24)=21, a(25)=22, a(26)=23
 * Range:     u8 n <= 317, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* 1‑based initial segment (indices 1..26).  Index 0 is unused. */
    static const seq_t init[27] = {
        0, /* 0 (unused) */
        2, 2, 3, 4, 5,
        6, 7, 8, 8, 9,
        10, 11, 12, 13, 14,
        14, 15, 16, 17, 18,
        18, 19, 20, 21, 22, 23
    };

    if (n <= 26) {
        return init[(size_t)n];
    }

    /* Reduce by 5 until we land in the init table, accumulating +4 per step. */
    idx_t idx = n;
    seq_t offset = 0;
    while (idx > 26) {
        idx -= 5;
        offset += 4;
    }

    return init[(size_t)idx] + offset;
}
