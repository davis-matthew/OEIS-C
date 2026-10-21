/*
 * A243889  Primes of the form 2*n^2+30*n+13.
 * https://oeis.org/A243889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 17, u32 n <= 18, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence of primes of the form 2*n^2+30*n+13.
   The function tests integer values m, evaluates 2*m*m+30*m+13,
   keeps the primes, and returns the nth such prime. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* External types defined by the build system */
typedef seq_t seq_t;
typedef idx_t idx_t;

seq_t a(idx_t n)
{
    /* Handle invalid index */
    if (n == 0)
        return 0;

    /* Memoization state */
    static seq_t *primes   = NULL;   /* stored primes */
    static size_t primes_cap = 0;    /* allocated capacity */
    static idx_t count = 0;          /* number of primes stored (1‑based) */
    static idx_t last_m = 11;        /* last m examined (next starts at 12) */

    /* If the requested term is already known, return it */
    if ((size_t)n <= count)
        return primes[n - 1];

    /* Ensure sufficient storage */
    if (primes_cap < (size_t)n) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 16;
        if (new_cap < (size_t)n)
            new_cap = n;
        seq_t *new_ptr = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
        primes = new_ptr;
        primes_cap = new_cap;
    }

    /* Generate primes of the quadratic until the requested index is reached */
    while (count < n) {
        ++last_m;                     /* m starts from 12 */
        seq_t val = (seq_t)2 * last_m * last_m
                    + (seq_t)30 * last_m + (seq_t)13;
        if (is_prime_big(val)) {
            primes[count++] = val;
        }
    }

    return primes[n - 1];
}
