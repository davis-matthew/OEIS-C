/*
 * A301971  a(n) = [x^n] Product_{k>=1} 1/(1 - x^prime(k))^n.
 * https://oeis.org/A301971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the coefficient of x^n in the product ∏_{p prime} (1 -
 *            x^p)^(-n)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1 is defined.  The function computes a(n) for any n ≥ 0. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---- prime sieve with caching ------------------------------------------- */
static uint64_t *prime_list = NULL;     /* array of primes  */
static size_t   prime_cnt  = 0;         /* number of primes in prime_list */
static idx_t    sieve_lim  = 0;         /* largest n for which sieve has been built */

static void ensure_primes(idx_t n)
{
    if (n <= sieve_lim) return;

    size_t lim = (size_t)n;
    bool *is_prime = (bool*)malloc((lim + 1) * sizeof(bool));
    if (!is_prime) return;   /* allocation failed – should not happen for small n */

    for (size_t i = 0; i <= lim; ++i) is_prime[i] = true;
    if (lim >= 0) is_prime[0] = false;
    if (lim >= 1) is_prime[1] = false;

    for (size_t p = 2; p * p <= lim; ++p)
    {
        if (is_prime[p])
        {
            for (size_t m = p * p; m <= lim; m += p)
                is_prime[m] = false;
        }
    }

    size_t cnt = 0;
    for (size_t i = 2; i <= lim; ++i)
        if (is_prime[i]) ++cnt;

    uint64_t *new_list = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!new_list) { free(is_prime); return; }

    size_t pos = 0;
    for (size_t i = 2; i <= lim; ++i)
        if (is_prime[i]) new_list[pos++] = (uint64_t)i;

    free(prime_list);
    free(is_prime);

    prime_list = new_list;
    prime_cnt  = cnt;
    sieve_lim  = n;
}

/* ---- main sequence function --------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    ensure_primes(n);

    size_t N = (size_t)n;                 /* n as size_t for array indices   */
    size_t sz = N + 1;                    /* array size                     */

    seq_t *dp    = (seq_t*)malloc(sz * sizeof(seq_t));
    seq_t *newdp = (seq_t*)malloc(sz * sizeof(seq_t));
    if (!dp || !newdp) { return (seq_t)0; }  /* memory failure – return 0 */

    for (size_t i = 0; i < sz; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    const seq_t Nseq = (seq_t)n;

    for (size_t pi = 0; pi < prime_cnt && prime_list[pi] <= n; ++pi)
    {
        idx_t p = (idx_t)prime_list[pi];

        /* copy current dp to newdp */
        for (size_t s = 0; s < sz; ++s) newdp[s] = dp[s];

        seq_t binom = (seq_t)1; /* binom(n + j - 1, j) for j = 0 initially */

        for (size_t j = 1; ; ++j)
        {
            size_t pj = p * j;
            if (pj > N) break;

            /* compute next binomial coefficient: C(n + j - 1, j) */
            binom = (binom * (Nseq + (seq_t)(j - 1))) / (seq_t)j;

            for (size_t s = pj; s < sz; ++s)
            {
                newdp[s] += dp[s - pj] * binom;
            }
        }

        /* swap dp and newdp for next iteration */
        seq_t *tmp = dp; dp = newdp; newdp = tmp;
    }

    seq_t result = dp[N];
    free(dp);
    free(newdp);
    return result;
}
