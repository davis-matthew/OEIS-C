/*
 * A332334  Let a(1) = a(2) = 1, and for n > 2 let a(n) = p where p is the
 *          largest prime such that p# divides phi(n), where phi is Euler's
 *          totient function and # is the primorial.
 * https://oeis.org/A332334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime p such that primorial(p) divides φ(n), with
 *            a(1)=a(2)=1
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   build_primes_upto, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/phi.h"
#include "../utils/build_primes_upto.h"

/* Static storage for the list of primes up to the largest φ(n) seen so far */
static uint64_t *primes = NULL;
static size_t prime_count = 0;
static idx_t prime_limit = 0;

/* Ensure that primes[] contains all primes ≤ limit */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit) return;
    if (primes) free(primes);
    build_primes_upto(limit, &primes, &prime_count);
    prime_limit = limit;
}

seq_t a(idx_t n)
{
    if (n <= 2) return (seq_t)1;

    seq_t phi_val = phi(n);
    idx_t phi_idx = (idx_t)phi_val;           /* φ(n) fits in idx_t for n ≤ 10000 */
    ensure_primes(phi_idx);

    seq_t last = (seq_t)2;                    /* at least 2 divides φ(n) for n>2 */
    for (size_t i = 0; i < prime_count; ++i)
    {
        uint64_t p = primes[i];
        if (phi_val % (seq_t)p == 0)
            last = (seq_t)p;
        else
            break;
    }
    return last;
}
