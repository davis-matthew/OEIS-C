/*
 * A285966  {11->1}-transform of the Thue-Morse word A010060.
 * https://oeis.org/A285966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Thue‑Morse sequence with every run of consecutive 1s
 *            reduced to a single 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    int prev = 0;                     /* previous bit of the Thue–Morse word */
    for (idx_t i = 0; ; ++i) {
        /* Thue–Morse bit at position i: parity of set bits of i (0 or 1) */
        int bit = __builtin_parityll((uint64_t)i);

        /* output the bit unless it is a second 1 of a run */
        if (!(bit == 1 && prev == 1)) {
            ++count;
            if (count == n) {
                return (seq_t)bit;
            }
        }
        prev = bit;
    }
}
