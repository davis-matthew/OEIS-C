/*
 * A289866  Primes obtained from other primes by prefixing a 1.
 * https://oeis.org/A289866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth prime obtained by prefixing the digit 1 to a
 *            prime p and the result is also prime
 * Range:     u8 n <= 9, u16 n <= 231, u32 n <= 138, u64 n <= 2170,
 *            u128 n <= 2170, big any n
 * Helpers:   build_primes_upto, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/pow10.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Persistent state for memoization */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;       /* number of terms stored */
    static size_t seq_cap = 0;       /* current capacity of seq */

    static uint64_t *primes = NULL;  /* array of primes */
    static size_t prime_cnt = 0;     /* number of primes in the array */
    static size_t prime_index = 0;   /* next prime to process */
    static uint64_t prime_limit = 0; /* highest limit used for primes */

    /* One‑time initialization */
    if (prime_limit == 0) {
        prime_limit = 1000000ULL;                /* start with 1 000 000 */
        build_primes_upto(prime_limit, &primes, &prime_cnt);
        seq_cap = 32;
        seq = (seq_t *)malloc(sizeof(seq_t) * seq_cap);
        seq_len = 0;
    }

    /* Generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        if (prime_index >= prime_cnt) {          /* need more primes */
            uint64_t new_limit = prime_limit * 2ULL;
            free(primes);
            build_primes_upto(new_limit, &primes, &prime_cnt);
            prime_limit = new_limit;
        }

        uint64_t p = primes[prime_index++];     /* next prime p */

        /* count digits of p */
        uint64_t tmp = p;
        uint64_t d = 0;
        do { ++d; tmp /= 10; } while (tmp);

        /* construct q = 10^d + p  (prefixing a 1) */
        seq_t q = pow10((idx_t)d) + (seq_t)p;

        /* test whether q is prime */
        if (is_prime_big(q)) {
            if (seq_len >= seq_cap) {           /* grow storage */
                seq_cap = seq_cap * 2;
                seq = (seq_t *)realloc(seq, sizeof(seq_t) * seq_cap);
            }
            seq[seq_len++] = q;
        }
    }

    return seq[(size_t)n - 1];
}
