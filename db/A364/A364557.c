/*
 * A364557  Möbius transform of A005941.
 * https://oeis.org/A364557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^((# of 2's in n)-1 + Σ_{odd primes p^e} (e-1) +
 *            (π(max_odd_prime)-1)), where π(x) is the number of primes ≤ x.
 *            This follows from the Möbius transform of A005941 and the
 *            identities of the involved OEIS sequences
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 136, u64 n <= 312,
 *            u128 n <= 726, big any n
 * Helpers:   build_primes_upto, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1)                 /* a(1) = 1 */
        return 1;

    /* Static data for prime sieve and π(·) prefix table */
    static bool initialized = false;
    static idx_t max_limit = 0;
    static uint64_t *primes = NULL;   /* list of primes up to max_limit */
    static size_t prime_cnt = 0;      /* number of primes */
    static unsigned *pi_arr = NULL;   /* π(x) for 0 ≤ x ≤ max_limit */

    /* Re‑build data if a larger n is requested */
    if (!initialized || n > max_limit) {
        if (max_limit > 0) {
            free(primes);
            free(pi_arr);
        }

        build_primes_upto(n, &primes, &prime_cnt);

        pi_arr = (unsigned *)calloc((size_t)n + 1, sizeof(unsigned));
        if (!pi_arr)
            return 0;   /* allocation failure → return 0 */

        for (size_t i = 0; i < prime_cnt; ++i)
            pi_arr[primes[i]] = 1;          /* mark primes */

        for (size_t i = 1; i <= (size_t)n; ++i)
            pi_arr[i] += pi_arr[i - 1];      /* prefix counts → π(i) */

        max_limit = n;
        initialized = true;
    }

    /* Factorisation of n */
    idx_t temp = n;
    idx_t exp2 = 0;
    while (temp && ((temp & 1) == 0)) {  /* count factors of 2 */
        ++exp2;
        temp >>= 1;
    }

    idx_t exponent = 0;
    if (exp2 >= 1)                         /* (a-1) part for 2^a */
        exponent += exp2 - 1;

    idx_t sum_e_minus1 = 0;                /* Σ(e_i-1) over odd primes */
    idx_t max_odd_prime = 0;               /* largest odd prime factor */

    while (temp > 1) {
        idx_t p = 0;
        idx_t e = 0;

        /* Find smallest prime divisor of temp */
        for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= temp; ++i) {
            if (temp % primes[i] == 0) {
                p = primes[i];
                break;
            }
        }

        if (p == 0) {                 /* temp itself is prime */
            p = temp;
            e = 1;
            temp = 1;
        } else {                       /* count exponent of p */
            while (temp % p == 0) {
                ++e;
                temp /= p;
            }
        }

        sum_e_minus1 += e - 1;
        if (p > max_odd_prime)
            max_odd_prime = p;
    }

    exponent += sum_e_minus1;              /* add all (e_i-1) terms */
    if (max_odd_prime > 0)
        exponent += (idx_t)(pi_arr[max_odd_prime] - 1);  /* (π(p_max)-1) */

    return pow2(exponent);
}
