/*
 * A354346  a(n) = 2*n - A064989(sigma(sigma(A003961(n)))), where A003961
 *          shifts the prime factorization one step towards larger primes, and
 *          A064989 shifts it back towards smaller primes.
 * https://oeis.org/A354346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*n - A064989( sigma( sigma( A003961(n) ) ) )
 * Range:     i8 n <= 11, i16 n <= 755, i32 n <= 9436, i64 n <= 9436,
 *            i128 n <= 9436, big any n
 * Helpers:   factor_u64, next_prime, prev_prime, seq_pow, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/prev_prime.h"
#include "../utils/sigma_of.h"
#include "../utils/seq_pow.h"

/* A003961: shift the exponents in n's prime factorization to the next prime. */
static seq_t shift_to_larger(idx_t n)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t np = next_prime(p);
        seq_t term = seq_pow((seq_t)np, (idx_t)exps[i]); /* np^exps[i] */
        prod *= term;
    }
    return prod;
}

/* A064989: shift the exponents in x's prime factorization to the previous prime
   (dropping the factor 2). */
static seq_t shift_to_smaller(seq_t x)
{
    /* Expect x to fit in 64 bits for the range used by the sequence. */
    uint64_t n = (uint64_t)x;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        if (p == 2)  /* drop the smallest prime (2) */
            continue;
        uint64_t lp = prev_prime(p);
        seq_t term = seq_pow((seq_t)lp, (idx_t)exps[i]); /* lp^exps[i] */
        prod *= term;
    }
    return prod;
}

seq_t a(idx_t n)
{
    seq_t m  = shift_to_larger(n);          /* A003961(n)   */
    seq_t s1 = sigma_of((idx_t)m);          /* sigma(...)   */
    seq_t s2 = sigma_of((idx_t)s1);         /* sigma(...)   */
    seq_t b  = shift_to_smaller(s2);        /* A064989(...) */
    return (seq_t)n * (seq_t)2 - b;         /* 2*n - b      */
}
