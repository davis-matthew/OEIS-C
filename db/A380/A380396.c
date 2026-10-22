/*
 * A380396  a(n) is the sum of the unitary divisors of n that are cubes.
 * https://oeis.org/A380396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} (1+p^e) if e≡0 (mod 3), else 1 This is the sum
 *            of the unitary divisors of n that are perfect cubes
 * Range:     u8 n <= 342, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, but guard for safety */
    seq_t result = 1;
    uint64_t m = (uint64_t)n;   /* use 64‑bit integer for factorisation */

    /* handle factor 2 separately */
    if ((m & 1) == 0) {
        idx_t exp = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            exp++;
        }
        if (exp % 3 == 0) {
            seq_t pow = 1;
            seq_t p   = 2;
            for (idx_t i = 0; i < exp; ++i) pow *= p;
            result *= (pow + 1);
        }
    }

    /* odd prime factors */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            if (exp % 3 == 0) {
                seq_t pow = 1;
                seq_t sp  = p;
                for (idx_t i = 0; i < exp; ++i) pow *= sp;
                result *= (pow + 1);
            }
        }
    }

    /* remaining prime factor (exponent 1, no effect) */
    return result;
}
