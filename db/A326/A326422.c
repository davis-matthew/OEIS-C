/*
 * A326422  Numbers k such that A000045(k) mod 5 is prime.
 * https://oeis.org/A326422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*floor((n-1)/8) + [3,4,6,7,13,14,16,17][(n-1)%8]
 * Range:     u8 n <= 102, u16 n <= 3740, u32 n <= 3740, u64 n <= 3740,
 *            u128 n <= 3740, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 is assumed */
    idx_t m = n - 1;
    idx_t q = m / 8;      /* number of full periods (each of length 20) */
    idx_t r = m % 8;      /* position inside the current period (0‑based) */

    static const seq_t small[8] = {
        (seq_t)3, (seq_t)4, (seq_t)6, (seq_t)7,
        (seq_t)13, (seq_t)14, (seq_t)16, (seq_t)17
    };

    seq_t res = (seq_t)20 * (seq_t)q + small[r];
    return res;
}
