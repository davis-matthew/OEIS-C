/*
 * A336381  Primes p(n) such that gcd(n, prime(n-1)+prime(n+1)) > 1.
 * https://oeis.org/A336381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n), where the prime satisfies gcd(n,
 *            prime(n-1)+prime(n+1)) > 1 (OEIS A336381)
 * Range:     u8 n <= 21, u16 n <= 3982, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* ------------------------------------------------------------------ */
/* Prime storage ----------------------------------------------------- */
/* primes[k]  = prime(k+1)   (0‑based indexing)                      */
/* ------------------------------------------------------------------ */

static uint64_t *primes = NULL;        /* array of primes */
static size_t   primes_len = 0;        /* number of primes stored */
static size_t   primes_cap = 0;        /* allocated capacity */

/* Add the next prime after the current primes array.           */
static void add_next_prime (void)
{
    /* start searching from the next odd number after the last prime    */
    uint64_t cand;
    if (primes_len == 0) {
        cand = 2;        /* first prime */
    } else {
        cand = primes[primes_len - 1] + 1;
        if ((cand & 1ULL) == 0) cand++;
    }

    for (;;) {
        bool prime = true;
        /* trial division using the primes already found            */
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t p = primes[i];
            /* stop when p*p > cand; use division to avoid overflow  */
            if (p > cand / p)
                break;
            if (cand % p == 0) {
                prime = false;
                break;
            }
        }
        if (prime)
            break;             /* cand is prime */
        cand += 2;             /* skip even numbers */
    }

    /* store the newly found prime */
    if (primes_len == primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 4;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        if (!tmp)           /* memory failure – give up */
            return;
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[primes_len++] = cand;
}

/* ------------------------------------------------------------------ */
/* Result sequence storage ------------------------------------------ */
/* sequence[k] = a(k+1)   (0‑based indexing)                         */
/* ------------------------------------------------------------------ */

static seq_t *sequence = NULL;        /* array of sequence terms */
static size_t seq_len = 0;            /* number of terms stored */
static size_t seq_cap = 0;            /* allocated capacity */

/* Append a new term to the result sequence. */
static void append_sequence (seq_t value)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 4;
        seq_t *tmp = (seq_t *)realloc(sequence, new_cap * sizeof(seq_t));
        if (!tmp)            /* memory failure – give up */
            return;
        sequence = tmp;
        seq_cap = new_cap;
    }
    sequence[seq_len++] = value;
}

/* ------------------------------------------------------------------ */
/* Main sequence function ------------------------------------------- */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* out of range – not used by OEIS */

    /* Ensure the primes array starts with 2 and 3. */
    if (primes_len == 0) {
        primes_cap = 4;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        if (!primes)
            return (seq_t)0;                  /* allocation failure */
        primes[0] = 2;
        primes[1] = 3;
        primes_len = 2;
    }

    /* Generate primes and sequence terms until we have the nth term. */
    while (seq_len < (size_t)n) {
        /* Add next prime and examine the new index. */
        add_next_prime();                        /* primes_len increased by 1 */

        /* We need at least 3 primes to evaluate a position. */
        if (primes_len < 3)
            continue;

        /* n_index is the 1‑based index of the newly added prime. */
        seq_t n_index = (seq_t)(primes_len - 1);

        /* Compute prime(n-1)+prime(n+1) using the surrounding primes. */
        seq_t sum = (seq_t)primes[primes_len - 3] + (seq_t)primes[primes_len - 1];

        /* Check the gcd condition. */
        if (gcd(n_index, sum) > (seq_t)1) {
            seq_t prime_n = (seq_t)primes[primes_len - 2];
            append_sequence(prime_n);
        }
    }

    return sequence[n - 1];
}
