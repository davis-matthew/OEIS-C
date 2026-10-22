/*
 * A320523  Smallest m > 1 such that either n^m == n (mod 25) or n^m == 0 (mod
 *          25).
 * https://oeis.org/A320523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the smallest integer m > 1 such that either * n^m ≡ n
 *            (mod 25) or n^m ≡ 0 (mod 25). For n not divisible by 5 * this
 *            equals the multiplicative order of n modulo 25 plus 1
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce the index modulo 25 */
    idx_t r = n % 25ULL;

    /* n ≡ 0 (mod 5)  →  n^2 ≡ 0 (mod 25), so m = 2 */
    if (r == 0 || r % 5 == 0)
        return (seq_t)2;

    /* Compute the multiplicative order of r modulo 25 */
    seq_t base = (seq_t)r;
    seq_t pow  = base;
    seq_t e    = 1;

    while (pow % 25 != 1) {
        pow = (pow * base) % 25;
        ++e;
    }

    /* The required m is the order plus one */
    return e + 1;
}
