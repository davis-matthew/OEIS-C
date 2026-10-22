/*
 * A378229  Dirichlet inverse of A341529, where A341529(n) = sigma(n) *
 *          A003961(n), and A003961 is fully multiplicative with a(prime(i)) =
 *          prime(i+1).
 * https://oeis.org/A378229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) is the Dirichlet inverse of A341529(n) where A341529(n) =
 *            sigma(n) * A003961(n) and A003961 is multiplicative with
 *            A003961(p) = nextprime(p)
 * Range:     i8 n <= 5, i16 n <= 69, i32 n <= 9902, i64 n <= 9902,
 *            i128 n <= 9902, big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>           /* size_t */
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"

/* Helper: next prime after x */
static idx_t next_prime_idx(idx_t x)
{
    for (idx_t y = x + 1; ; ++y)
        if (is_prime(y))
            return y;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* Factorisation of n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t pf = 0; pf < cnt; ++pf)
    {
        idx_t p   = (idx_t)primes[pf];
        int   k   = (int)exps[pf];
        idx_t np  = next_prime_idx(p);

        seq_t f_arr[64];          /* f(p^e) */
        seq_t a_arr[64];          /* a(p^e) */

        seq_t sigma = (seq_t)1;   /* sigma(p^0) */
        seq_t npow  = (seq_t)1;   /* np^0   */
        f_arr[0] = (seq_t)1;
        a_arr[0] = (seq_t)1;

        for (int e = 1; e <= k; ++e)
        {
            sigma = sigma * (seq_t)p + (seq_t)1;          /* sigma(p^e) */
            npow  = npow * (seq_t)np;                   /* np^e      */
            f_arr[e] = sigma * npow;                     /* f(p^e)    */
        }

        for (int e = 1; e <= k; ++e)
        {
            seq_t sum = (seq_t)0;
            for (int j = 1; j <= e; ++j)
                sum += f_arr[j] * a_arr[e - j];
            a_arr[e] = -sum;
        }

        result *= a_arr[k];
    }

    return result;
}
