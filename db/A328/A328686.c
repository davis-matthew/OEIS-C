/*
 * A328686  Define a map from the primes to the primes by f(p) = (p-1)/2 if
 *          that is prime, or else (p+1)/2 if that is prime, and otherwise is
 *          undefined. Start with the n-th prime and iterate f until we cannot
 *          go any further; a(n) is the number of steps.
 * https://oeis.org/A328686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A328686: a(n) = number of steps in the trajectory of the nth prime.
   Each step applies f(p) = (p-1)/2 if it is prime, else (p+1)/2 if that is prime.
   The process stops when neither value is prime. */
#include "../utils/is_prime.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* defensive guard for n<1 */
    idx_t p = (idx_t)get_prime(n);   /* nth prime (1‑based) */
    seq_t steps = 0;

    while (1) {
        idx_t p1 = (p - 1) / 2;
        if (is_prime(p1)) {          /* (p-1)/2 is prime */
            p = p1;
            steps++;
            continue;
        }

        idx_t p2 = (p + 1) / 2;
        if (is_prime(p2)) {          /* (p+1)/2 is prime */
            p = p2;
            steps++;
            continue;
        }

        break;                       /* no further prime transition */
    }
    return steps;
}
