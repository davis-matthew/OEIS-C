/*
 * A363823  Moving the rightmost digit of a number to place it furthest to the
 *          left adds 18 to the number.
 * https://oeis.org/A363823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = repdigit(d, L) – 20, where L and d are chosen so that the
 *            nth smallest number of the form repdigit(d, L)–20 with the
 *            digit‑count condition holds: * if n ≤ 7 : L = 2, d = n + 2
 *            (digits 3→9) * if 8 ≤ n ≤ 15 : L = 3, d = n – 6 (digits 2→9) *
 *            otherwise : let r = n – 16; L = 4 + r/9, d = r%9 +...
 * Range:     u8 n <= 8, u16 n <= 29, u32 n <= 72, u64 n <= 160, u128 n <= 333,
 *            big any n
 * Helpers:   repdigit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/repdigit.h"

seq_t a(idx_t n) {
    if (n == 0)          /* undefined for offset 1, but return 0 */
        return 0;

    idx_t L;
    uint64_t d;

    if (n <= 7) {                /* 2‑digit repdigits, d = 3…9 */
        L = 2;
        d = (uint64_t)(n + 2);
    }
    else if (n <= 15) {          /* 3‑digit repdigits, d = 2…9 */
        L = 3;
        d = (uint64_t)(n - 6);
    }
    else {                       /* 4‑digit and higher, d = 1…9 */
        idx_t r = n - 16;
        L = 4 + r / 9;
        d = (uint64_t)(r % 9 + 1);
    }

    seq_t rep = repdigit(d, L);
    return rep - (seq_t)20;
}
