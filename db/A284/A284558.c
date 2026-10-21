/*
 * A284558  Product / LCM of run lengths in binary representation of n: a(n) =
 *          A167489(n) / A284559(n).
 * https://oeis.org/A284558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A167489(n) / A284559(n) A167489(n) = product of run
 *            lengths in the binary expansion of n A284559(n) = lcm of those
 *            run lengths
 * Range:     u8 n <= 9705, u16 n <= 9705, u32 n <= 9705, u64 n <= 9705,
 *            u128 n <= 9705, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"


seq_t a(idx_t n)
{
    /* Special case n = 0 (binary representation "0") */
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t product = 1;
    seq_t lcm = 1;

    /* Count number of bits in n */
    idx_t bits = 0;
    idx_t tmp = n;
    while (tmp) {
        bits++;
        tmp >>= 1;
    }

    bool first = true;
    int prev_bit = 0;
    idx_t run_len = 0;

    /* Process bits from most significant to least */
    for (idx_t pos = bits; pos-- > 0; ) {
        int bit = (int)((n >> pos) & 1);

        if (first) {
            first = false;
            prev_bit = bit;
            run_len = 1;
            continue;
        }

        if (bit == prev_bit) {
            run_len++;
        } else {
            seq_t rl = (seq_t)run_len;
            product *= rl;
            lcm = lcm * rl / gcd(lcm, rl);
            prev_bit = bit;
            run_len = 1;
        }
    }

    /* Final run */
    seq_t rl = (seq_t)run_len;
    product *= rl;
    lcm = lcm * rl / gcd(lcm, rl);

    return product / lcm;
}
