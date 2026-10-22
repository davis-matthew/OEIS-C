/*
 * A369865  a(n) = n * Sum_{p|n, p prime} mu(n/p)^2 / p.
 * https://oeis.org/A369865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * sum_{p|n} μ(n/p)^2 / p
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n <= 1) return 0;                  /* a(1)=0 */

    /* Factorise n */
    idx_t temp = n;
    size_t cnt = 0;
    idx_t primes[64];
    unsigned int exps[64];

    /* Handle factor 2 */
    if ((temp & 1ULL) == 0) {
        unsigned int e = 0;
        while ((temp & 1ULL) == 0) {
            temp >>= 1;
            e++;
        }
        primes[cnt] = 2;
        exps[cnt++] = e;
    }

    /* Handle odd factors */
    for (idx_t i = 3; (unsigned __int128)i * i <= temp; i += 2) {
        if (temp % i == 0) {
            unsigned int e = 0;
            while (temp % i == 0) {
                temp /= i;
                e++;
            }
            primes[cnt] = i;
            exps[cnt++] = e;
        }
    }

    /* Any remaining prime > sqrt(n) */
    if (temp > 1) {
        primes[cnt] = temp;
        exps[cnt] = 1;
        cnt++;
    }

    /* Find primes with exponent > 1 */
    size_t big_count = 0;
    idx_t big_prime = 0;
    unsigned int big_exp = 0;

    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] > 1) {
            big_count++;
            if (big_count == 1) {
                big_prime = primes[i];
                big_exp = exps[i];
            }
        }
    }

    /* If more than one prime has exponent > 1, a(n)=0 */
    if (big_count > 1) return 0;

    if (big_count == 1) {
        /* Only the prime with exponent 2 contributes */
        if (big_exp != 2) return 0;
        return (seq_t)(n / big_prime);
    }

    /* All prime exponents are 1 (n is squarefree) */
    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        sum += (seq_t)(n / primes[i]);
    }
    return sum;
}
