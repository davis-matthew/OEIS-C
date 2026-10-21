/*
 * A296210  Characteristic function for A104210: a(n) = 1 if n is divisible by
 *          at least 2 consecutive primes, 0 otherwise.
 * https://oeis.org/A296210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is divisible by the product of at least two
 *            consecutive primes, 0 otherwise
 * Range:     u8 n <= 704, u16 n <= 8548, u32 n <= 8548, u64 n <= 8548,
 *            u128 n <= 8548, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- dynamic prime list (incremental sieve) ---------- */
static uint64_t *prime_list = NULL;          /* array of primes */
static size_t prime_cnt = 0;                 /* number of primes stored   */
static size_t prime_cap = 0;                 /* capacity of the array     */
static uint64_t next_candidate = 3;          /* next odd to test          */
static bool primes_initialized = false;

/* initialize prime array with first prime 2 */
static void init_primes(void)
{
    prime_cap = 8;
    prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    if (!prime_list) abort(); /* fail fast */
    prime_list[0] = 2;
    prime_cnt = 1;
    primes_initialized = true;
    next_candidate = 3;
}

/* generate the next prime and append it to prime_list */
static void generate_next_prime(void)
{
    if (!primes_initialized) init_primes();

    while (true) {
        bool is_prime = true;
        /* trial division by existing primes (up to sqrt(next_candidate)) */
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > next_candidate) break;
            if (next_candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap <<= 1;
                prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
                if (!prime_list) abort();
            }
            prime_list[prime_cnt++] = next_candidate;
            break;
        }
        next_candidate += 2; /* skip even numbers */
    }
    next_candidate += 2; /* advance to next candidate for future calls */
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* Numbers smaller than 6 cannot be divisible by a product of two consecutive primes. */
    if (n < 6) return (seq_t)0;

    seq_t nseq = (seq_t)n;
    size_t i = 0; /* index of the first prime in the pair */

    while (true) {
        /* Ensure we have at least two primes for the current pair. */
        while (prime_cnt <= i + 1) {
            generate_next_prime();
        }

        /* Compute the product of the consecutive primes. */
        __uint128_t prod128 = (__uint128_t)prime_list[i] * prime_list[i + 1];
        seq_t prod = (seq_t)prod128;

        /* If the product already exceeds n, no further pairs can divide n. */
        if (prod > nseq) {
            return (seq_t)0;
        }

        /* Check divisibility. */
        if (nseq % prod == 0) {
            return (seq_t)1;
        }

        /* Move to the next pair. */
        ++i;
    }
}
