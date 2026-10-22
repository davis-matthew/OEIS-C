/*
 * A321332  Duration of Morse code representation of n.
 * https://oeis.org/A321332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word, easy
 * Formula:   a(n) = 7*digits(n) - 3 + Σ (dots(d_i) + 3*dashes(d_i))
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Morse components for decimal digits 0‑9 */
    static const unsigned dot_count[10] = {0,1,2,3,4,5,4,3,2,1};
    static const unsigned dash_count[10] = {5,4,3,2,1,0,1,2,3,4};

    idx_t tmp = n;
    unsigned digits = 0;
    unsigned dots_total = 0;
    unsigned dashes_total = 0;

    if (tmp == 0) {
        digits = 1;
        dots_total = dot_count[0];
        dashes_total = dash_count[0];
    } else {
        while (tmp > 0) {
            unsigned d = (unsigned)(tmp % 10);
            digits++;
            dots_total += dot_count[d];
            dashes_total += dash_count[d];
            tmp /= 10;
        }
    }

    seq_t base   = 7 * ((seq_t)digits) - 3;
    seq_t dots   = (seq_t)dots_total;
    seq_t dashes = 3 * ((seq_t)dashes_total);
    return base + dots + dashes;
}
