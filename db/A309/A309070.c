/*
 * A309070  a(n) is the number of tiles of a chosen color on the front side of
 *          the Rubik Cube after n repetitions of the following procedure:
 *          rotate the right side by a 1/4 turn clockwise, then rotate the
 *          whole cube around the front-back axis by a 1/4 turn clockwise.
 * https://oeis.org/A309070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of tiles of a chosen color on the front side of a
 *            Rubik Cube after n repetitions of rotating the right face 1/4
 *            turn clockwise, then rotating the whole cube about the
 *            front‐back axis 1/4 turn clockwise. The sequence is periodic
 *            with period 60. Initial values: a(0)=9, a(1)=6, a(2)=4,...
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>


seq_t a(idx_t n)
{
    static seq_t table[60];
    static int initialized = 0;
    if (!initialized) {
        const seq_t init[24] = {
            9, 6, 4, 3, 1, 1, 2, 2, 3, 5,
            6, 6, 7, 4, 2, 2, 1, 2, 4, 4,
            5, 5, 4, 4
        };
        for (int i = 0; i < 24; ++i)
            table[i] = init[i];
        for (int i = 24; i < 60; ++i) {
            int val =
                - (int)table[i-2] - (int)table[i-4] - (int)table[i-6] - (int)table[i-8]
                + (int)table[i-12] + (int)table[i-14] + (int)table[i-16] + (int)table[i-18] + (int)table[i-20];
            table[i] = (seq_t)val;
        }
        initialized = 1;
    }
    return table[(size_t)(n % 60)];
}
