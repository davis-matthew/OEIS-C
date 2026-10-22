/*
 * A390941  "Late birds" in A390939: terms A390939(m) such that A390939(k) >
 *          A390939(m) for all k > m, where A390939 lists the keys added in
 *          the map initialized with T[1] = 1 and then repeatedly T[v] := k +
 *          (T[v] if defined else 0) for all key-value pairs (k, v) in T.
 * https://oeis.org/A390941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 961 ⋅ 2^(n-17) for n ≥ 22; a(1)=1, a(2)=2, a(3)=4,
 *            a(4)=6, a(5)=26, a(6)=52, a(7)=54, a(8)=66, a(9)=68, a(10)=72,
 *            a(11)=116, a(12)=232, a(13)=464, a(14)=928, a(15)=1856,
 *            a(16)=1922, a(17)=3842, a(18)=5764, a(19)=15372, a(20)=15376,
 *            a(21)=30744
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 39, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    static const seq_t first[21] = {
        1UL, 2UL, 4UL, 6UL, 26UL, 52UL, 54UL, 66UL, 68UL, 72UL,
        116UL, 232UL, 464UL, 928UL, 1856UL, 1922UL, 3842UL,
        5764UL, 15372UL, 15376UL, 30744UL
    };

    if (n <= 21)
        return first[n - 1];

    /* For n ≥ 22 use the closed form 961 * 2^(n–17) */
    return 961 * pow2(n - 17);
}
