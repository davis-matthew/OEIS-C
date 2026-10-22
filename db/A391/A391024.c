/*
 * A391024  Classification of numbers predicated according to prime powers,
 *          squarefree, powerful, and perfect powers.
 * https://oeis.org/A391024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd_int_local, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   Classification algorithm for OEIS sequence A391024.
   For each integer n ≥ 0, the function returns:

        0 : n = 0 or 1
        1 : n is a prime
        2 : n is a prime power with exponent > 1
        3 : n is a squarefree composite (all exponents = 1, at least 2 primes)
        4 : n has mixed exponents (some exponents = 1, some > 1) – not powerful
        5 : n is powerful (all exponents ≥ 2) but not a perfect power
        6 : n is powerful and a perfect power (gcd of exponents > 1)

   The implementation uses the utility routine factor_u64 to factor n.
   All intermediate results fit into 64‑bit integers.
*/

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/gcd_int_local.h"

seq_t a(idx_t n)
{
    /* Handle the offset : a(0) = a(1) = 0. */
    if (n <= 1u)
        return (seq_t)0;

    /* Primes get classification 1. */
    if (is_prime(n))
        return (seq_t)1;

    /* Factor n into primes and exponents. */
    uint64_t temp = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exp_u64[64];
    size_t cnt;

    factor_u64(temp, primes, exp_u64, &cnt);

    /* n is composite. */
    if (cnt == 1) {
        /* Single prime factor with exponent > 1 (prime power). */
        return (seq_t)2;
    }

    /* For multiples of at least two distinct primes examine exponents. */
    int all_one = 1;
    int all_ge_two = 1;
    int g = (int)exp_u64[0];

    for (size_t i = 0; i < cnt; ++i) {
        int e = (int)exp_u64[i];
        if (e != 1)
            all_one = 0;
        if (e < 2)
            all_ge_two = 0;
        g = gcd_int_local(g, e);
    }

    if (all_one)                 /* squarefree composite  */
        return (seq_t)3;

    if (all_ge_two) {            /* powerful number */
        if (g > 1)                /* perfect power          */
            return (seq_t)6;
        else                       /* powerful, not perfect  */
            return (seq_t)5;
    }

    /* Mixed exponents (not powerful, not squarefree). */
    return (seq_t)4;
}
