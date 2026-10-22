/*
 * A378615  Number of non prime powers <= prime(n).
 * https://oeis.org/A378615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   build_primes_upto, count_prime_powers, get_prime,
 *            prime_power_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A378615
 *  a(n) = prime(n) – (# of prime powers ≤ prime(n))
 *  The function obtains the nth prime with get_prime, then
 *  counts the prime powers up to that prime by enumerating all
 *  primes ≤ prime(n) (using build_primes_upto), counting p⁰
 *  for each prime and their higher powers.
 */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/build_primes_upto.h"
#include "../utils/count_prime_powers.h"
#include "../utils/prime_power_count_u64.h"

seq_t a(idx_t n)
{
    if (n == 0)        /* offset 1 – defined only for n ≥ 1 */
        return (seq_t)0;

    /* 1. Obtain the nth prime.  get_prime returns a 32‑bit value. */
    uint64_t prime_n = (uint64_t)get_prime(n);

    /* 2. Produce all primes ≤ prime_n.  build_primes_upto allocates
     *    an array of uint64_t and returns the count. */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto((idx_t)prime_n, &primes, &prime_cnt);

    /* 3. Count the prime powers up to prime_n. */
    size_t power_cnt = count_prime_powers(prime_n, primes, prime_cnt);

    /* 4. Free the sieve array. */
    free(primes);

    /* 5. Compute the sequence term.  All calculations are performed in
     *    native 64‑bit integers and then cast to seq_t. */
    seq_t result = (seq_t)prime_n - (seq_t)power_cnt;
    return result;
}
