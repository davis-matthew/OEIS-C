/*
 * A277594  Numbers k such that k/10^m == 7 mod 10, where 10^m is the greatest
 *          power of 10 that divides n.
 * https://oeis.org/A277594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 7279, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A277594:
   numbers k such that k/10^m ≡ 7 (mod 10), where 10^m is the greatest
   power of 10 dividing k.
   The function enumerates successive integers, removes trailing zeros,
   tests whether the last remaining digit is 7, and returns the n‑th
   number that satisfies the test. */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* offset starts at 1, 0 is never requested */

    idx_t count = 0;
    idx_t cur   = 1;          /* current integer being examined */

    for (;;) {
        idx_t x = cur;

        /* Strip trailing zeros. */
        while (x % 10 == 0)
            x /= 10;

        /* Check for rightmost non‑zero digit equal to 7. */
        if (x % 10 == 7) {
            ++count;
            if (count == n)
                return (seq_t)cur;   /* cast because cur is idx_t */
        }

        ++cur;
    }
}
