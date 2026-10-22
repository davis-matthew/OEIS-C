/*
 * A380092  Number of consecutive primes after prime(n) before their
 *          concatenation fails to produce a prime.
 * https://oeis.org/A380092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) is the number of successive primes after prime(n) whose
 *            cumulative concatenation remains a prime
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 10, u64 n <= 105, u128 n <= 105,
 *            big any n
 * Helpers:   digits, is_prime_big, pow10, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime_big.h"
#include "../utils/sieve_primes.h"
#include "../utils/pow10.h"
#include "../utils/digits.h"

/* Global state for prime generation */
static idx_t *prime_list   = NULL; /* array of primes, 0‑based */
static size_t prime_count  = 0;    /* number of primes stored */
static idx_t  prime_limit  = 0;    /* numeric limit discovered */

/* Ensure we have at least 'needed' primes (1‑based indexing). */
static void ensure_primes_up_to_count(idx_t needed) {
    if (prime_count >= (size_t)needed)
        return;

    if (prime_limit == 0)
        prime_limit = 2;

    while (prime_count < (size_t)needed) {
        prime_limit *= 2;      /* increase numeric upper bound */
        idx_t *tmp = NULL;
        size_t cnt = 0;
        sieve_primes(prime_limit, &tmp, &cnt);
        if (prime_list) free(prime_list);
        prime_list = tmp;
        prime_count = cnt;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1, but guard against 0 */
        return 0;

    /* We need at least p_n and p_{n+1} to start. */
    ensure_primes_up_to_count(n + 1);

    idx_t idx    = n - 1;          /* index of prime(n) in 0‑based array */
    seq_t concat = (seq_t)prime_list[idx]; /* current concatenation */
    seq_t count  = 0;              /* answer */

    idx_t next_idx = idx + 1;      /* next prime to try */

    while (true) {
        /* Make sure we have the next prime available. */
        if (next_idx >= prime_count)
            ensure_primes_up_to_count(next_idx + 1);

        /* Concatenate current value with the next prime. */
        int d    = digits(prime_list[next_idx]); /* digits of the prime */
        seq_t pow10_d = pow10((idx_t)d);
        seq_t new_concat = concat * pow10_d + (seq_t)prime_list[next_idx];

        /* Test primality of the new concatenation. */
        if (is_prime_big(new_concat)) {
            count++;            /* success */
            concat = new_concat;/* continue */
            next_idx++;
        } else {
            break;              /* failure */
        }
    }

    return count;
}
