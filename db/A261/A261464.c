/*
 * A261464  a(n) = prime(n+2)^3 - prime(n+1)^2 + prime(n).
 * https://oeis.org/A261464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   big_pow, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = prime(n+2)^3 - prime(n+1)^2 + prime(n)
 *
 * Implements the OEIS sequence A261464.
 * Uses a small incremental prime table and the provided big_pow helper
 * for exact integer arithmetic.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/is_prime.h"

/* The build supplies the following typedefs:
 *   typedef <inttype> idx_t;   // unsigned 64‑bit index
 *   typedef <inttype> seq_t;   // unsigned integer, 8/16/32/64/128 bits or arbitrary‑precision
 */

/* ----- Prime storage ----------------------------------------------------- */
static uint64_t *primes = NULL;   /* 0‑based array of primes */
static size_t    primes_cnt = 0;  /* number of primes stored */
static size_t    primes_cap = 0;  /* allocated capacity */

/* ----- Helper: extend the prime table until at least 'needed' primes ----- */
static void ensure_primes(idx_t needed)
{
    /* Nothing to do if we already have enough primes. */
    if ((size_t)needed <= primes_cnt)
        return;

    /* Allocate or grow the prime array. */
    if (primes == NULL) {
        primes_cap = 128;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        if (!primes) exit(EXIT_FAILURE);
    }

    /* Generate primes until we have 'needed' of them. */
    while (primes_cnt < needed) {
        uint64_t cand = primes_cnt == 0 ? 2ULL : primes[primes_cnt - 1] + 1;

        /* Skip even numbers after 2. */
        if (cand > 2ULL && (cand & 1ULL) == 0ULL)
            ++cand;

        /* Find the next prime. */
        while (true) {
            bool prime = true;
            for (size_t i = 0; i < primes_cnt; ++i) {
                uint64_t p = primes[i];
                if (p * p > cand)
                    break;
                if (cand % p == 0ULL) {
                    prime = false;
                    break;
                }
            }
            if (prime)
                break;
            cand += 2; /* advance to the next odd number */
        }

        /* Store the prime. */
        if (primes_cnt == primes_cap) {
            primes_cap <<= 1; /* double capacity */
            uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
            if (!tmp) exit(EXIT_FAILURE);
            primes = tmp;
        }
        primes[primes_cnt++] = cand;
    }
}

/* ----- The sequence function ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n < 1 */

    /* We need primes up to index n+2 (1‑based). */
    ensure_primes(n + 2);

    /* Retrieve the required primes. */
    seq_t p_n  = (seq_t)primes[(size_t)(n - 1)];     /* prime(n)   */
    seq_t p_n1 = (seq_t)primes[(size_t) n ];        /* prime(n+1) */
    seq_t p_n2 = (seq_t)primes[(size_t)(n + 1)];    /* prime(n+2) */

    /* Compute the formula using seq_t arithmetic. */
    seq_t cube   = big_pow(p_n2, (seq_t)3);
    seq_t square = p_n1 * p_n1;
    seq_t val    = cube - square + p_n;
    return val;
}
