/*
 * A323399  Sum of Jordan function J_2(n), A007434 and its Dirichlet inverse,
 *          A046970.
 * https://oeis.org/A323399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = J_2(n) + g(n) where J_2(n) = n^2 * Π_{p|n} (1 - 1/p^2)
 *            (Jordan totient of order 2) g(n) = Π_{p|n} (-(p^2 - 1))
 *            (Dirichlet inverse of J_2)
 * Range:     u8 n <= 13, u16 n <= 184, u32 n <= 801, u64 n <= 9931,
 *            u128 n <= 9931, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/build_primes_upto.h"

static idx_t *primes = NULL;
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (primes != NULL)
        return;
    idx_t limit = 1000000;            /* sufficient for n up to ~10^12          */
    size_t cnt;
    build_primes_upto(limit, &primes, &cnt);
    prime_cnt = cnt;
}

seq_t a(idx_t n)
{
    init_primes();

    seq_t j2      = (seq_t)n * (seq_t)n;   /* initial value n²                    */
    seq_t prod_g  = 1;                   /* Π (p² - 1) over distinct primes    */
    idx_t distinct = 0;                   /* number of distinct prime factors    */

    idx_t m = n;
    for (size_t i = 0; i < prime_cnt && (idx_t)primes[i] * primes[i] <= m; ++i)
    {
        idx_t p = primes[i];
        if (m % p == 0)
        {
            /* distinct prime factor detected */
            ++distinct;
            seq_t p2 = (seq_t)p * p;
            prod_g  *= (p2 - 1);

            /* multiply J_2(n) by (p² - 1)/p²          */
            j2 = j2 * (p2 - 1) / p2;

            while (m % p == 0)
                m /= p;
        }
    }
    if (m > 1)                       /* remaining prime factor */
    {
        ++distinct;
        seq_t p2 = (seq_t)m * m;
        prod_g  *= (p2 - 1);
        j2 = j2 * (p2 - 1) / p2;
    }

    /* g(n) = (-1)^distinct * prod_g;  a(n) = J_2(n) + g(n)                */
    if ((distinct & 1) == 0)
        return j2 + prod_g;
    else
        return j2 - prod_g;
}
