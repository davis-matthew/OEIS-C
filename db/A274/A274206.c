/*
 * A274206  a(n) = the last nonzero digit of n followed by all the trailing
 *          zeros of n.
 * https://oeis.org/A274206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(10*n) = 10*a(n); a(10*n + k) = k (1 ≤ k ≤ 9)
 * Range:     u8 n <= 299, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t temp = n;
    idx_t t = 0;
    /* Count trailing zeros and find the last non‑zero digit */
    while ((temp % 10) == 0) {
        temp /= 10;
        ++t;
    }
    idx_t d = temp % 10;     /* last non‑zero digit (1‑9) */

    seq_t result = (seq_t)d; /* result starts with the last non‑zero digit */

    /* Append all the trailing zeros by multiplying with 10^t */
    while (t--) {
        result *= 10;
    }

    return result;
}
