/*
 * A307551  Number of iterations of the map of quadratic residues x -> x^2
 *          (mod prime(n)) with the initial term x = n^2 (mod prime(n)) needed
 *          to reach the end of the cycle.
 * https://oeis.org/A307551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   sqr_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: A307551   a(n) = number of iterations of x -> x^2 (mod prime(n))
 * starting from x = n^2 (mod prime(n)) until the first repeated value.
 * Return value = length of transient + cycle length - 1.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/sqr_mod.h"

/* Find the n-th prime (1‑based). */
static idx_t nth_prime(idx_t n)
{
    static uint64_t primes[20000];
    static idx_t count = 0;

    if (n == 0)
        return 0;

    while (count < n) {
        uint64_t candidate = count == 0 ? 2 : primes[count - 1] + 1;
        if (candidate < 2)
            candidate = 2;

        for (;;) {
            bool prime = true;
            for (idx_t i = 0; i < count; ++i) {
                uint64_t p = primes[i];
                if ((unsigned __int128)p * p > (unsigned __int128)candidate)
                    break;
                if (candidate % p == 0) {
                    prime = false;
                    break;
                }
            }
            if (prime) {
                primes[count++] = candidate;
                break;
            }
            ++candidate;
        }
    }
    return primes[n - 1];
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Prime, initial value */
    idx_t p_idx = nth_prime(n);
    seq_t prime  = (seq_t)p_idx;
    seq_t x0     = ((seq_t)n * (seq_t)n) % prime;

    /* Floyd’s cycle‑finding algorithm */
    seq_t tortoise = sqr_mod(x0, prime);
    seq_t hare     = sqr_mod(sqr_mod(x0, prime), prime);
    while (tortoise != hare) {
        tortoise = sqr_mod(tortoise, prime);
        hare     = sqr_mod(sqr_mod(hare, prime), prime);
    }

    /* Find start of cycle (mu) */
    seq_t mu = 0;
    tortoise = x0;
    while (tortoise != hare) {
        tortoise = sqr_mod(tortoise, prime);
        hare     = sqr_mod(hare, prime);
        mu++;
    }

    /* Find cycle length (lambda) */
    seq_t lambda = 1;
    hare = sqr_mod(tortoise, prime);
    while (tortoise != hare) {
        hare = sqr_mod(hare, prime);
        lambda++;
    }

    /* Total distinct values before repeat = mu + lambda */
    return (mu + lambda) - 1;
}
