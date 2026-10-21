/*
 * A249675  Multinomial coefficient (2+3+...+prime(n); 2,3,5,7,...,prime(n)) =
 *          A007504(n)! / A111180(n).
 * https://oeis.org/A249675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (sum_{k=1..n} p_k)! / (∏_{k=1..n} p_k!)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   build_primes, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where p_k is the k-th prime.  */

/* Required helper headers */
#include "../utils/build_primes.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

/* LEGENDRE’s formula: exponent of prime p in m! */
static inline idx_t exp_in_factorial(idx_t m, idx_t p)
{
    idx_t exp = 0;
    for (idx_t div = p; div <= m; div *= p)
    {
        exp += m / div;
        if (div > m / p) break;  /* avoid overflow of div */
    }
    return exp;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for 0, harmless */

    /* ---- obtain first n primes ---------------------------------------------------- */
    idx_t limit = n * 20 + 100;            /* rough upper bound for nth prime       */
    idx_t *first_primes = NULL;            /* will be overwritten in loop           */
    size_t prime_cnt = 0;

    while (prime_cnt < n)
    {
        build_primes(limit, &first_primes, &prime_cnt);
        if (prime_cnt < n)                 /* insufficient, double limit again      */
            limit <<= 1;
    }

    /* ---- sum of first n primes ----------------------------------------------- */
    idx_t sum_primes = 0;
    for (idx_t i = 0; i < n; ++i)
        sum_primes += first_primes[i];

    /* ---- all primes up to the sum --------------------------------------------- */
    idx_t *all_primes = NULL;
    size_t all_cnt   = 0;
    build_primes(sum_primes, &all_primes, &all_cnt);

    /* ---- compute multinomial coefficient --------------------------------------- */
    seq_t result = (seq_t)1;

    for (size_t i = 0; i < all_cnt; ++i)
    {
        idx_t q = all_primes[i];

        /* exponent of q in (sum_primes)! */
        idx_t e_num = exp_in_factorial(sum_primes, q);

        /* subtract exponents from each prime factorial */
        idx_t e_den = 0;
        for (idx_t j = 0; j < n; ++j)
            e_den += exp_in_factorial(first_primes[j], q);

        idx_t e = e_num - e_den;
        if (e == 0) continue;

        seq_t pow_q = seq_pow((seq_t)q, e);
        result = seq_mul(result, pow_q);
    }

    return result;
}
