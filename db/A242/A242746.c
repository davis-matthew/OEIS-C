/*
 * A242746  Least k > 0 such that (k!+n!)/(k+n) is an integer.
 * https://oeis.org/A242746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k > 0 such that (k! + n!)/(k + n) is an integer
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/*
 * Computes a(n) for the OEIS sequence A242746.
 * The function returns the least positive integer k such that
 *   (k! + n!) % (k + n) == 0
 * For n >= 1.  It makes use of the helper function is_prime from
 * ../utils/is_prime.h and relies only on integer arithmetic.
 */
seq_t a(idx_t n)
{
    /* Handle the special case given by the sequence definition */
    if (is_prime(n + 1))
        return 1;

    /* Search for the minimal k in the interval [1, n] */
    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t m = k + n;          /* modulus (k + n) */

        /* Compute k! modulo m */
        idx_t fk = 1;
        for (idx_t i = 1; i <= k; ++i)
            fk = (fk * i) % m;

        /* Compute n! modulo m */
        idx_t fn = 1;
        for (idx_t i = 1; i <= n; ++i)
            fn = (fn * i) % m;

        /* Check divisibility: (k! + n!) % (k + n) == 0 */
        if (((fk + fn) % m) == 0)
            return (seq_t)k;  /* minimal k found */
    }

    /* By construction k = n is always a valid solution, so the loop
       above must have returned.  This return is only for completeness. */
    return (seq_t)n;
}
