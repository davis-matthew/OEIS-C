/*
 * A329557  Smallest MM-number of a set of n nonempty sets.
 * https://oeis.org/A329557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Smallest MM-number of a set of n nonempty sets (OEIS A329557).
 * a(0) = 1, and for n >= 1
 * a(n) = product of the first n primes whose indices (starting from 2) are
 * square‑free integers.  That is,
 * let s_k be the k‑th integer > 1 that is square‑free;
 * then a(n) = ∏_{k=1}^{n} prime(s_k).
 * This formula reproduces all terms listed in the OEIS entry.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* We need the (n+1)-th square‑free index > 1.
       A crude upper bound is 2*n + 10.  We will sieve up to a limit
       that is comfortably larger than that number of primes.           */
    idx_t max_idx   = n * 2 + 10;
    idx_t sieve_lim = max_idx * 20 + 100;   /* plenty of room for primes */

    /* ---------- generate primes up to sieve_lim --------------------- */
    bool *is_prime = (bool*)malloc(sieve_lim + 1);
    for (idx_t i = 0; i <= sieve_lim; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t i = 2; i * i <= sieve_lim; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= sieve_lim; j += i) is_prime[j] = false;
        }
    }

    /* Collect prime values into an array.  The size is generous. */
    size_t primes_capacity = sieve_lim / 2 + 10;
    uint64_t *primes = (uint64_t*)malloc(primes_capacity * sizeof(uint64_t));
    size_t primes_cnt = 0;
    for (idx_t i = 2; i <= sieve_lim; ++i) {
        if (is_prime[i]) {
            if (primes_cnt == primes_capacity) {
                primes_capacity *= 2;
                primes = (uint64_t*)realloc(primes,
                                            primes_capacity * sizeof(uint64_t));
            }
            primes[primes_cnt++] = (uint64_t)i;
        }
    }

    /* ---------- mark square‑free numbers up to sieve_lim ------------- */
    bool *squarefree = (bool*)malloc(sieve_lim + 1);
    for (idx_t i = 0; i <= sieve_lim; ++i) squarefree[i] = true;
    for (idx_t p = 2; p * p <= sieve_lim; ++p) {
        if (is_prime[p]) {
            idx_t sq = p * p;
            for (idx_t j = sq; j <= sieve_lim; j += sq) squarefree[j] = false;
        }
    }

    /* ---------- accumulate product of first n square‑free indices ----- */
    seq_t product = (seq_t)1;
    idx_t count = 0;
    for (idx_t i = 2; i <= sieve_lim && count < n; ++i) {
        if (squarefree[i]) {
            /* prime at index i is primes[i-1] (since primes[0] = 2). */
            uint64_t prime_val = primes[i - 1];
            product = product * prime_val;
            ++count;
        }
    }

    /* ---------- cleanup -------------------------------------------- */
    free(is_prime);
    free(squarefree);
    free(primes);

    return product;
}
