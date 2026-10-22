/*
 * A343231  A binary encoding of the nonzero digits in balanced ternary
 *          representation of n.
 * https://oeis.org/A343231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = mask of the non‑zero digits in the balanced ternary
 *            representation of n. The i‑th bit of a(n) is set iff the i‑th
 *            digit (least significant first) in the balanced ternary
 *            representation of n equals +1 or –1
 * Range:     u8 n <= 3280, u16 n <= 6561, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;    /* output value */
    seq_t bit     = 1;    /* current bit position */

    while (n != 0) {
        idx_t r = n % 3;
        n /= 3;

        /* A balanced ternary digit of 1 or –1 contributes a bit. */
        if (r == 1 || r == 2) {
            result |= bit;
        }

        /* Remainder 2 represents –1 → carry 1 to the next digit. */
        if (r == 2) {
            n += 1;
        }

        bit <<= 1;
    }

    return result;
}
