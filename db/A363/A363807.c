/*
 * A363807  Number of divisors of n of the form 7*k + 5.
 * https://oeis.org/A363807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of n that are ≡ 5 (mod 7)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case n == 1 */
    if (n == 1)
        return (seq_t)0;

    /* Count divisors by prime factorization and DP over residues modulo 7 */
    seq_t count_mod[7] = {0};
    count_mod[1] = 1;                    /* divisor 1 has residue 1 mod 7 */

    idx_t tmp = n;
    /* Trial division to factor n */
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p != 0)
            continue;
        /* Count exponent of p */
        idx_t exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }
        /* Precompute powers of p modulo 7 up to exp */
        int pmod = (int)(p % 7);
        int pow_mod = 1;
        int pow_mods[64]; /* exp <= 20 for n <= 10^12, enough for 10^4 */
        pow_mods[0] = 1;
        for (idx_t k = 1; k <= exp; ++k) {
            pow_mod = (pow_mod * pmod) % 7;
            pow_mods[k] = pow_mod;
        }

        /* Update DP table */
        seq_t new_mod[7] = {0};
        for (int r = 0; r < 7; ++r) {
            if (count_mod[r] == 0)
                continue;
            for (idx_t k = 0; k <= exp; ++k) {
                int new_r = (r * pow_mods[k]) % 7;
                new_mod[new_r] += count_mod[r];
            }
        }
        for (int r = 0; r < 7; ++r)
            count_mod[r] = new_mod[r];
    }

    /* If remaining tmp > 1, it is a prime factor with exponent 1 */
    if (tmp > 1) {
        int pmod = (int)(tmp % 7);
        seq_t new_mod[7] = {0};
        for (int r = 0; r < 7; ++r) {
            if (count_mod[r] == 0)
                continue;
            /* exponent 0 */
            new_mod[r] += count_mod[r];
            /* exponent 1 */
            int new_r = (r * pmod) % 7;
            new_mod[new_r] += count_mod[r];
        }
        for (int r = 0; r < 7; ++r)
            count_mod[r] = new_mod[r];
    }

    return count_mod[5];
}
