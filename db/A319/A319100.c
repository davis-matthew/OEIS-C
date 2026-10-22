/*
 * A319100  Number of solutions to x^6 == 1 (mod n).
 * https://oeis.org/A319100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = number of solutions to x^6 == 1 (mod n)
 * Range:     u8 n <= 2183, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    seq_t res = (seq_t)1;
    idx_t tmp = n;

    /* Factor 2 */
    idx_t e = 0;
    while ((tmp & 1ULL) == 0) {
        e++;
        tmp >>= 1;
    }
    if (e > 0) {
        if (e == 1)
            res *= (seq_t)1;
        else if (e == 2)
            res *= (seq_t)2;
        else
            res *= (seq_t)4;
    }

    /* Factor 3 */
    e = 0;
    while (tmp % 3ULL == 0) {
        e++;
        tmp /= 3;
    }
    if (e > 0) {
        if (e == 1)
            res *= (seq_t)2;
        else
            res *= (seq_t)6;
    }

    /* Factor remaining odd primes */
    for (idx_t p = 5; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            seq_t contrib = (p % 6 == 1) ? (seq_t)6 : (seq_t)2;
            res *= contrib;
            while (tmp % p == 0)
                tmp /= p;
        }
    }

    if (tmp > 1) {
        seq_t contrib = (tmp % 6 == 1) ? (seq_t)6 : (seq_t)2;
        res *= contrib;
    }

    return res;
}
