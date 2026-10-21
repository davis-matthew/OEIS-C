/*
 * A245344  Sum of digits of n written in fractional base 7/3.
 * https://oeis.org/A245344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of digits of the unique representation of n in base
 *            7/3 (digits 0..6)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t r   = n;
    const idx_t P = 7, Q = 3, INVQ = 5;  /* 5 is the modular inverse of 3 modulo 7 */

    while (r > 0) {
        idx_t r_mod = r % P;               /* r mod 7 */
        idx_t rem   = (Q * r_mod) % P;     /* (q * r) mod p */
        idx_t d     = (rem * INVQ) % P;    /* digit in [0,6] */

        sum += (seq_t)d;

        idx_t diff = r - d;                /* r - d is ≥ 0 */
        idx_t r_prime = diff / P;          /* integer part of (r-d)/7 */
        idx_t rem2    = diff % P;          /* remainder < 7 */

        r = r_prime * Q + (rem2 * Q) / P;  /* (q * (r - d)) / p */
    }

    return sum;
}
