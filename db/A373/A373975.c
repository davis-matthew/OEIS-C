/*
 * A373975  a(n) = 1 if the number of prime factors with multiplicity is a
 *          multiple of 3, otherwise 0.
 * https://oeis.org/A373975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if Ω(n) ≡ 0 (mod 3), otherwise 0
 * Range:     u8 n <= 9411, u16 n <= 9411, u32 n <= 9411, u64 n <= 9411,
 *            u128 n <= 9411, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Ω(1) = 0 by convention, so a(1) = 1 */
    if (n == 1)
        return (seq_t)1;

    idx_t t = n;
    int mod = 0;            /* Ω(n) modulo 3 */

    for (idx_t p = 2; p <= t / p; ++p) {
        while (t % p == 0) {
            mod = (mod + 1) % 3;
            t /= p;
        }
    }
    if (t > 1)                /* remaining prime factor */
        mod = (mod + 1) % 3;

    return (seq_t)(mod == 0 ? 1 : 0);
}
