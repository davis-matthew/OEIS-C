/*
 * A259265  Primes of the form Phi(5,n!), where Phi is the cyclotomic
 *          polynomial.
 * https://oeis.org/A259265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes of the form Φ5(k!) = k!^4 + k!^3 + k!^2 + k! + 1
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   ensure_capacity, is_prime_big, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    static seq_t *primes = NULL;
    static size_t primes_len = 0;
    static size_t primes_cap = 0;
    static idx_t next_k = 0;          /* k = next factorial index (0,1,2,…) */
    static seq_t fact = 0;            /* current k! value */
    static bool initialized = false;

    if (!initialized) {
        initialized = true;
        fact = (seq_t)1;   /* 0! = 1 */
        next_k = 0;
    }

    /* Generate primes until we have at least n of them */
    while (primes_len < (size_t)n) {
        /* Compute Φ5(k!) = k!^4 + k!^3 + k!^2 + k! + 1 */
        seq_t fact2 = seq_mul(fact, fact);          /* (k!)^2 */
        seq_t fact3 = seq_mul(fact2, fact);         /* (k!)^3 */
        seq_t fact4 = seq_mul(fact3, fact);         /* (k!)^4 */
        seq_t value = fact4 + fact3 + fact2 + fact + (seq_t)1;

        if (is_prime_big(value)) {
            /* Grow the array if needed */
            ensure_capacity(primes_len + 1, &primes_cap,
                            (void **)&primes, sizeof(seq_t));
            primes[primes_len++] = value;
        }

        /* Move to the next factorial: (k+1)! = k! * (k+1) */
        next_k++;
        seq_t nk = (seq_t)next_k;
        fact = seq_mul(fact, nk);
    }

    return primes[n - 1];
}
