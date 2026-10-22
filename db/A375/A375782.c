/*
 * A375782  The 2-adic valuation of the result, starting from n, of Collatz
 *          steps x -> (3x+1)/2 while odd.
 * https://oeis.org/A375782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the 2‑adic valuation of the first even number * obtained
 *            by iterating the Collatz map x → (3x+1)/2 as long as x is odd. *
 *            For even n this is simply v₂(n). For odd n the map is applied
 *            until * an even value appears, then the exponent of 2 dividing
 *            that value is * returned
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type for arithmetic. */
    seq_t x = (seq_t)n;

    /* Easy case: n is even – just count trailing zeros. */
    if ((x % 2) == 0) {
        seq_t cnt = 0;
        while ((x % 2) == 0) {
            cnt++;
            x /= 2;
        }
        return cnt;
    }

    /* Odd case: apply the Collatz step while the current value is odd. */
    while ((x % 2) != 0) {
        x = (x * 3 + 1) / 2;
    }

    /* Now x is even.  Count the 2‑adic valuation. */
    seq_t cnt = 0;
    while ((x % 2) == 0) {
        cnt++;
        x /= 2;
    }
    return cnt;
}
