/*
 * A384058  The number of integers k from 1 to n such that the greatest
 *          divisor of k that is a unitary divisor of n is a 5-rough number
 *          (A007310).
 * https://oeis.org/A384058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} (p^e – 1 if p ≤ 3; p^e if p ≥ 5)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;                /* a(1) = 1 */

    seq_t res = 1;
    idx_t   t   = n;

    /* factor 2 */
    if (t % 2 == 0) {
        seq_t pow = 1;
        while (t % 2 == 0) {
            t   /= 2;
            pow *= (seq_t)2;
        }
        res *= (pow - (seq_t)1);
    }

    /* factor 3 */
    if (t % 3 == 0) {
        seq_t pow = 1;
        while (t % 3 == 0) {
            t   /= 3;
            pow *= (seq_t)3;
        }
        res *= (pow - (seq_t)1);
    }

    /* factor odd primes ≥5 */
    for (idx_t p = 5; p <= t / p; p += 2) {      /* avoid overflow of p*p */
        if (t % p == 0) {
            seq_t pow = 1;
            while (t % p == 0) {
                t   /= p;
                pow *= (seq_t)p;
            }
            res *= pow;                          /* p ≥ 5 → contribution = p^e */
        }
    }

    /* any remaining prime factor > √t */
    if (t > 1)
        res *= (seq_t)t;

    return res;
}
