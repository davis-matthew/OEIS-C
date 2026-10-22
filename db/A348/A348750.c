/*
 * A348750  a(n) = A064989(A064989(sigma(A003961(A003961(n))))), where A003961
 *          shifts the prime factorization of n one step towards larger
 *          primes, and A064989 shifts it back towards smaller primes.
 * https://oeis.org/A348750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, next_prime, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = A064989( A064989( sigma( A003961( A003961(n) ) ) ) )
 * Function a(n) implements the multiplicative definition:
 *   a(p^e) = A064989( A064989( (q^(e+1)-1)/(q-1) ) ),
 * where q = nextPrime( nextPrime(p) ).
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/prev_prime.h"

/* Shift the prime factorization of m one step toward smaller primes.
 *  For each prime r > 2 in m, replace r by prev_prime(r);
 *  factors of 2 are dropped (since they map to 1).
 *  The result fits in uint64_t for all inputs arising here.
 */
static uint64_t shift_down_once(uint64_t m)
{
    uint64_t primes[32], exps[32];
    size_t count;

    /* m may be 0 or 1 → empty factorization */
    factor_u64(m, primes, exps, &count);

    uint64_t res = 1;
    for (size_t i = 0; i < count; ++i) {
        uint64_t prime = primes[i];
        if (prime == 2)    /* skip 2 (maps to 1) */
            continue;

        uint64_t prev = prev_prime(prime);
        unsigned int exp = (unsigned int)exps[i];
        for (unsigned int k = 0; k < exp; ++k)
            res *= prev;
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined, return 0 for safety */
        return 0;

    /* Factor n */
    uint64_t primes_n[32], exps_n[32];
    size_t cnt_n;
    factor_u64((uint64_t)n, primes_n, exps_n, &cnt_n);

    seq_t result = 1;

    for (size_t i = 0; i < cnt_n; ++i) {
        uint64_t p   = primes_n[i];
        unsigned int e = (unsigned int)exps_n[i];

        /* Determine q = nextPrime(nextPrime(p)) */
        uint64_t q1 = next_prime(p);
        uint64_t q  = next_prime(q1);

        /* Compute val = (q^(e+1)-1)/(q-1) */
        __int128 pow = 1;
        for (unsigned int j = 0; j < e + 1; ++j)
            pow *= q;
        uint64_t val = (uint64_t)((pow - 1) / (q - 1));

        /* Apply A064989 twice */
        uint64_t t1 = shift_down_once(val);
        uint64_t t2 = shift_down_once(t1);

        /* Accumulate product */
        result *= (seq_t)t2;
    }

    return result;
}
