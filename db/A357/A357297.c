/*
 * A357297  T(m,n) is the number of linear extensions of n fork-join DAGs of
 *          width m, read by downward antidiagonals.
 * https://oeis.org/A357297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (n*(m+2))! / ((m+1)^n * (m+2)^n) where (m,n) are mapped
 *            from n by antidiagonals
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 28, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   big_pow, fact_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/fact_exp.h"

/* Prime sieve up to a modest bound (enough for all possible factorial arguments). */
#define MAX_PRIME 20000
static int primes[MAX_PRIME + 1];
static size_t prime_cnt = 0;
static void init_primes(void)
{
    if (prime_cnt != 0)
        return;
    bool is_comp[MAX_PRIME + 1] = { false };
    for (int p = 2; p <= MAX_PRIME; ++p) {
        if (!is_comp[p]) {
            primes[prime_cnt++] = p;
            if ((long long)p * p <= MAX_PRIME) {
                for (int q = p * p; q <= MAX_PRIME; q += p)
                    is_comp[q] = true;
            }
        }
    }
}

/* Exponent of prime p in n! (Legendre's formula). */

/* Exponent of prime p in integer x. */
static idx_t prime_exp(idx_t x, int p)
{
    idx_t e = 0;
    while (x % p == 0) {
        x /= p;
        ++e;
    }
    return e;
}

/* Map index n to coordinates (m, nfork) on the antidiagonal. */
static void coords_from_index(idx_t n, idx_t *m_out, idx_t *nfork_out)
{
    idx_t s = 0;
    while (((s + 1) * (s + 2)) / 2 <= n)
        ++s;
    idx_t prev_total = s * (s + 1) / 2;
    idx_t m = n - prev_total;          /* offset within diagonal (row index) */
    idx_t nfork = s - m;                /* column index */
    *m_out = m;
    *nfork_out = nfork;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    init_primes();

    /* Determine (m, nfork) for index n. */
    idx_t m, nfork;
    coords_from_index(n, &m, &nfork);

    /* Special case: nfork == 0 => value is 1. */
    if (nfork == 0)
        return (seq_t)1;

    /* Compute argument of the factorial: arg = nfork * (m + 2). */
    idx_t arg = nfork * (m + 2);

    seq_t result = 1;

    /* For each prime <= arg, compute net exponent in the result. */
    for (size_t i = 0; i < prime_cnt && primes[i] <= arg; ++i) {
        int p = primes[i];

        /* Exponent in factorial(arg). */
        idx_t e_num = fact_exp(arg, p);

        /* Exponent in (m+1)^nfork * (m+2)^nfork. */
        idx_t e_den = nfork * (prime_exp(m + 1, p) + prime_exp(m + 2, p));

        /* Net exponent to multiply into result. */
        idx_t e = e_num - e_den;
        if (e > 0)
            result *= big_pow((seq_t)p, e);
    }

    return result;
}
