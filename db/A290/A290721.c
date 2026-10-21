/*
 * A290721  a(n) = 4^n - n - 1.
 * https://oeis.org/A290721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - n - 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base   = 4;          /* 4 as base for exponentiation */

    idx_t k = n;               /* local copy for bit‑wise processing */

    while (k > 0) {
        if (k & 1)             /* current bit is set: multiply into result */
            result *= base;

        k >>= 1;               /* move to next bit */
        if (k == 0)            /* no more bits → no need to square base */
            break;

        base *= base;          /* square base for next bit position */
    }

    /* subtract n + 1, casting n to seq_t before use */
    return result - (seq_t)n - 1;
}
