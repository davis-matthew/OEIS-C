/*
 * A286898  ASCII codes of letter keys by rows on standard U.S. "QWERTY"
 *          keyboard.
 * https://oeis.org/A286898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) = A002252(n) + 64 (ASCII code of the nth letter key in
 *            QWERTY order)
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    /* QWERTY order:  Q W E R T Y U I O P   A S D F G H J K L   Z X C V B N M
       Alphabet indices (A=1): 17 23 5 18 20 25 21 9 15 16 1 19 4 6 7 8 10 11 12 26 24 3 22 2 14 13 */
    static const seq_t MAP[26] = {
        17,23,5,18,20,25,21,9,15,16,1,19,4,6,7,8,10,11,12,26,24,3,22,2,14,13
    };
    /* n ≥ 1, (n-1) % 26 is in [0,25]; cast to size_t for array indexing */
    size_t idx = (size_t)((n - 1) % 26);
    return MAP[idx] + 64;
}
