/*
 * A273810  Primes of the form 2^(2^n) + 597.
 * https://oeis.org/A273810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime of the form 2^(2^k)+597, where k starts at
 *            0 and is increased until the n-th prime is found
 * Range:     u8 none, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   is_prime_big, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes 2^(2^k) using pow2(k) to get 2^k,
   then pow2((idx_t)2^k) to obtain 2^(2^k).  It caches found primes
   so subsequent calls are fast. */
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/pow2.h"
#include "../utils/is_prime_big.h"

extern seq_t pow2(idx_t exp);            /* computes 2^exp */
bool is_prime_big(seq_t n);              /* primality test for big ints */

seq_t a(idx_t n) {
    static seq_t *primes = NULL;
    static size_t primes_cap = 0;
    static idx_t primes_count = 0;
    static idx_t last_k = (idx_t)-1; /* last k processed */

    /* Grow array helper */
    if (!primes) {
        primes_cap = 8;
        primes = (seq_t*)calloc(primes_cap, sizeof(seq_t));
    }

    /* Search for primes until we have enough to satisfy n */
    for (idx_t k = last_k + 1; primes_count < n; ++k) {
        /* Compute 2^k */
        seq_t exp = pow2((idx_t)k);

        /* If exp exceeds uint64_t range, we cannot proceed safely */
        if (exp > (seq_t)UINT64_MAX) {
            /* No more computable k in this implementation */
            break;
        }

        /* Compute 2^(2^k) */
        idx_t exp_u64 = (idx_t)exp; /* safe due to the above check */
        seq_t val = pow2(exp_u64);

        /* Add 597 */
        val = val + (seq_t)597;

        /* Check primality */
        if (is_prime_big(val)) {
            if (primes_count == primes_cap) {
                primes_cap *= 2;
                primes = (seq_t*)realloc(primes, primes_cap * sizeof(seq_t));
            }
            primes[primes_count++] = val;
            if (primes_count >= n) {
                last_k = k;
                return val;
            }
        }
        last_k = k;
    }

    /* If we exit the loop without enough primes, return 0 */
    return (seq_t)0;
}
