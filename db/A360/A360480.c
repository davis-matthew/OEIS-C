/*
 * A360480  a(n) = number of numbers k < n, with gcd(k, n) > 1, such that
 *          there is at least one prime divisor p | k that does not divide n,
 *          and at least one prime divisor q | n that does not divide k.
 * https://oeis.org/A360480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k < n such that gcd(k,n) > 1, rad(k) and rad(n)
 *            are incomparable under division. Implementation: For each n
 *            factorize n and iterate k=2..n-1, checking the two inclusion
 *            conditions on prime divisors. Prime factorization is done by
 *            trial division using a small primes sieve up to √n
 * Range:     u8 n <= 436, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, gcd, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/int_sqrt.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n) {
    if (n <= 1) return 0;

    /* Small primes up to sqrt(n) for factorization */
    idx_t limit = int_sqrt(n);
    uint64_t *smallprimes = NULL;
    size_t smallcnt = 0;
    build_primes_upto(limit, &smallprimes, &smallcnt);

    /* Factorise n into distinct prime divisors */
    uint64_t primes_n[32];
    size_t cnt_n = 0;
    idx_t temp = n;
    for (size_t i = 0; i < smallcnt && (idx_t)smallprimes[i] * (idx_t)smallprimes[i] <= temp; i++) {
        uint64_t p = smallprimes[i];
        if (temp % p == 0) {
            primes_n[cnt_n++] = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) primes_n[cnt_n++] = temp;  /* remaining prime > sqrt(n) */

    seq_t count = 0;

    /* Iterate over all k < n */
    for (idx_t k = 2; k < n; k++) {
        if (gcd(k, n) == 1) continue;  /* gcd(k,n) must be > 1 */

        /* Factorise k into distinct prime divisors */
        uint64_t primes_k[32];
        size_t cnt_k = 0;
        idx_t tempk = k;
        for (size_t i = 0; i < smallcnt && (idx_t)smallprimes[i] * (idx_t)smallprimes[i] <= tempk; i++) {
            uint64_t pk = smallprimes[i];
            if (tempk % pk == 0) {
                primes_k[cnt_k++] = pk;
                while (tempk % pk == 0) tempk /= pk;
            }
        }
        if (tempk > 1) primes_k[cnt_k++] = tempk;

        /* Check if k has a prime not in n */
        bool k_has_new = false;
        for (size_t i = 0; i < cnt_k; i++) {
            uint64_t pk = primes_k[i];
            bool inN = false;
            for (size_t j = 0; j < cnt_n; j++)
                if (pk == primes_n[j]) { inN = true; break; }
            if (!inN) { k_has_new = true; break; }
        }
        if (!k_has_new) continue;  /* rad(k) ⊆ rad(n) */

        /* Check if n has a prime not in k */
        bool n_has_new = false;
        for (size_t j = 0; j < cnt_n; j++) {
            uint64_t pj = primes_n[j];
            bool inK = false;
            for (size_t i = 0; i < cnt_k; i++)
                if (pj == primes_k[i]) { inK = true; break; }
            if (!inK) { n_has_new = true; break; }
        }
        if (!n_has_new) continue;  /* rad(n) ⊆ rad(k) */

        /* Both conditions satisfied */
        count++;
    }

    free(smallprimes);
    return count;
}
