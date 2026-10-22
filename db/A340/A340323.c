/*
 * A340323  Multiplicative with a(p^e) = (p + 1) * (p - 1)^(e - 1).
 * https://oeis.org/A340323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^e‖n} (p + 1) * (p - 1)^(e - 1) (multiplicative)
 * Range:     u8 n <= 63, u16 n <= 9359, u32 n <= 9359, u64 n <= 9359,
 *            u128 n <= 9359, big any n
 * Helpers:   build_primes, floor_sqrt, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes.h"
#include "../utils/floor_sqrt.h"
#include "../utils/pow_int.h"

static idx_t *primes = NULL;      /* array of primes */
static size_t prime_cnt = 0;      /* number of primes in the array */
static idx_t prime_limit = 0;     /* current upper limit of the sieve */

/* Ensure that primes up to 'limit' are available.  The function
   rebuilds the table whenever the limit increases. */
static void
ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    /* build_primes allocates a new array; free the old one first */
    idx_t *new_primes = NULL;
    size_t new_cnt = 0;
    build_primes(limit, &new_primes, &new_cnt);

    free(primes);
    primes = new_primes;
    prime_cnt = new_cnt;
    prime_limit = limit;
}

/* The multiplicative function a(n) described above. */
seq_t
a(idx_t n)
{
    seq_t result = (seq_t)1;

    if (n == 1)
        return result;

    /* Determine the maximum prime needed for trial division.  Use
       floor_sqrt to avoid floating point. */
    idx_t max_prime = (idx_t)floor_sqrt((seq_t)n);
    ensure_primes(max_prime);

    idx_t m = n;
    for (size_t i = 0; i < prime_cnt && (idx_t)(primes[i]) * (idx_t)(primes[i]) <= m; ++i) {
        idx_t p = primes[i];
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t term = (seq_t)(p + 1);
            if (e > 1) {
                seq_t pow_val = pow_int((seq_t)(p - 1), e - 1);
                term = term * pow_val;
            }
            result *= term;
        }
    }

    /* If there is any remaining factor greater than sqrt(n), it is
       a prime with exponent 1. */
    if (m > 1) {
        seq_t term = (seq_t)(m + 1);
        result *= term;
    }

    return result;
}
