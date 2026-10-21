/*
 * A299644  a(n) = prime(prime(n+1)) + prime(prime(n)).
 * https://oeis.org/A299644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(prime(n+1)) + prime(prime(n))
 * Range:     u8 n <= 10, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by pre‑computing all primes up to a
   fixed bound that covers the required indices for n up to about
   10000.  The sieve is executed only once on first call. */
#include <stddef.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* static state */
    static bool initialized = false;
    static idx_t *primes   = NULL;   /* array of primes, 0‑based */
    static size_t prime_cnt = 0;

    /* For n up to 10000 the 1st prime at index prime(10001)
       is < 1.2 million, so any bound > 1.3 million is safe.
       A slightly larger constant keeps the sieve fast. */
    const idx_t LIMIT = 2000000U;

    if (!initialized) {
        build_primes_upto(LIMIT, &primes, &prime_cnt);
        initialized = true;
    }

    if (n == 0)          /* undefined for n<1, return 0 as safety */
        return 0;

    /* Retrieve primes: primes[i] is the (i+1)-th prime. */
    idx_t p_n   = primes[n - 1];          /* prime(n)   */
    idx_t p_n1  = primes[n];              /* prime(n+1) */

    /* Compute prime(prime(n)) and prime(prime(n+1)) */
    seq_t ppn   = (seq_t)primes[p_n  - 1];
    seq_t ppn1  = (seq_t)primes[p_n1 - 1];

    return ppn + ppn1;
}
