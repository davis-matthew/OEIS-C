/*
 * A300955  In the prime tower factorization of n, replace 2's with 3's and
 *          3's with 2's.
 * https://oeis.org/A300955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(1)=1; a(n)=∏_{p^e‖n} f(p)^{a(e)}, where f(2)=3, f(3)=2, f(p)=p
 *            for p>3
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 1249,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_seq, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"
#include "../utils/seq_mul.h"

static seq_t a_rec(idx_t n)
{
    if (n == 1)
        return 1;

    seq_t result = 1;
    idx_t temp = n;

    /* handle factor 2 */
    if (temp % 2 == 0) {
        idx_t exp = 0;
        while (temp % 2 == 0) {
            temp /= 2;
            exp++;
        }
        seq_t base = 3;               /* f(2) = 3 */
        seq_t exponent = a_rec(exp);  /* a(exp) */
        result = seq_mul(result, pow_seq(base, (int)exponent));
    }

    /* handle factor 3 */
    if (temp % 3 == 0) {
        idx_t exp = 0;
        while (temp % 3 == 0) {
            temp /= 3;
            exp++;
        }
        seq_t base = 2;               /* f(3) = 2 */
        seq_t exponent = a_rec(exp);  /* a(exp) */
        result = seq_mul(result, pow_seq(base, (int)exponent));
    }

    /* handle remaining odd primes */
    for (idx_t p = 5; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            seq_t base = (seq_t)p;          /* f(p) = p for p>3 */
            seq_t exponent = a_rec(exp);
            result = seq_mul(result, pow_seq(base, (int)exponent));
        }
    }

    /* leftover prime factor > sqrt(n) */
    if (temp > 1) {
        /* temp is a prime > 3 */
        seq_t base = (seq_t)temp;  /* f(p) = p */
        result = seq_mul(result, base);  /* exponent = 1 */
    }

    return result;
}

seq_t a(idx_t n)
{
    return a_rec(n);
}
