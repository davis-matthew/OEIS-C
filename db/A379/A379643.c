/*
 * A379643  List of x coordinates of prime numbers in a Cartesian grid, where
 *          the first prime 2 is placed at the origin (0,0) and the second
 *          prime 3 at (1,0). For the n-th prime prime(n), n >= 3, take a unit
 *          step in the direction (prime(n)-3)*45 degrees counterclockwise
 *          from the positive x-axis.
 * https://oeis.org/A379643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = pi_{8,3}(prime_n) - pi_{8,7}(prime_n)
 * Range:     i8 n <= 86, i16 n <= 86, i32 n <= 86, i64 n <= 86, i128 n <= 86,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* static state shared across calls for memoisation */
static idx_t   *primes = NULL;  /* array of primes, 0‑indexed */
static seq_t   *cumul  = NULL;  /* cumulative value a(i+1) stored at i */
static size_t    count  = 0;     /* number of primes currently stored */
static size_t    cap    = 0;     /* allocated capacity of arrays */

/* ensure the internal arrays can hold at least 'size' entries */
static void grow_capacity(size_t size)
{
    if (cap == 0) {
        cap = 8;
    }
    while (cap < size) {
        cap <<= 1;
    }
    /* realloc must be cast in C++ */
    primes  = (idx_t*)realloc(primes, cap * sizeof(idx_t));
    cumul   = (seq_t*)realloc(cumul,  cap * sizeof(seq_t));
}

/* public function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* sequence only defined for n>=1 */

    if (count == 0) {                      /* first call: initialise */
        grow_capacity(1);
        primes[0]   = 2;
        cumul[0]    = 0;                   /* a(1) = 0 */
        count      = 1;
    }

    /* extend precomputation until we have at least n primes */
    idx_t candidate = (count == 1 ? 3 : primes[count-1] + 2);

    while (count < n) {
        /* find next prime */
        while (!is_prime(candidate)) {
            candidate += 2;                /* stay on odd numbers only */
        }

        /* ensure capacity for the new prime */
        if (count >= cap) {
            grow_capacity(count + 1);
        }

        /* store prime and update cumulative value */
        primes[count] = candidate;

        seq_t inc = 0;
        idx_t mod = candidate % 8;
        if (mod == 3) inc = 1;
        else if (mod == 7) inc = -1;
        cumul[count] = cumul[count-1] + inc;

        /* prepare for next iteration */
        ++count;
        candidate += 2;                    /* next odd candidate */
    }

    return cumul[n-1];                     /* a(1) stored at index 0 */
}
