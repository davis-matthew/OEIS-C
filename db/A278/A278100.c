/*
 * A278100  Number of squarefree positive integers less than n^2.
 * https://oeis.org/A278100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = \sum_{d=1}^{n-1} μ(d) * ⌊(n²-1)/d²⌋ Number of square‑free
 *            positive integers < n²
 * Range:     u8 n <= 16, u16 n <= 327, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/*  μ(d) precomputation (linear sieve)                                */
/* ------------------------------------------------------------------ */
static int8_t *mu_tbl = NULL;   /* mu_tbl[i] = μ(i) */
static idx_t   mu_max = 0;      /* largest n for which mu_tbl is valid */

/* ensure mu_tbl contains μ(i) for all i ≤ n (inclusive) */
static void ensure_mu(idx_t n)
{
    if (n <= mu_max)
        return;                           /* already computed */

    /* allocate / reallocate storage for μ up to n */
    size_t sz   = (size_t)n + 1;
    int8_t *old = mu_tbl;
    mu_tbl      = (int8_t *)realloc(mu_tbl, sz * sizeof(int8_t));
    if (!mu_tbl)
        abort();                           /* allocation failure */

    /* auxiliary arrays for linear sieve */
    uint8_t *is_comp = (uint8_t *)calloc(sz, sizeof(uint8_t));
    int     *primes  = (int *)malloc(sz * sizeof(int));
    size_t   prim_cnt = 0;

    if (!is_comp || !primes)
    {
        free(is_comp);
        free(primes);
        free(old);
        abort();
    }

    /* initialise μ[0] and μ[1] */
    mu_tbl[0] = 0;
    if (n >= 1)
        mu_tbl[1] = 1;

    /* linear sieve up to n */
    for (size_t i = 2; i <= (size_t)n; ++i)
    {
        if (!is_comp[i])
        {
            primes[prim_cnt++] = (int)i;
            mu_tbl[i] = -1;                /* prime → μ = -1 */
        }
        for (size_t j = 0; j < prim_cnt; ++j)
        {
            size_t p   = (size_t)primes[j];
            size_t prod = p * i;
            if (prod > (size_t)n)
                break;
            is_comp[prod] = 1;
            if (i % p == 0)
            {
                mu_tbl[prod] = 0;          /* square factor → μ = 0 */
                break;
            }
            else
            {
                mu_tbl[prod] = -mu_tbl[i];
            }
        }
    }

    free(is_comp);
    free(primes);

    mu_max = n;
}

/* ------------------------------------------------------------------ */
/*  OEIS sequence a(n)                                                */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* offset: a(1) = 0 */
    if (n < 2)
        return 0;

    /* we need μ(d) for d = 1 … n-1 */
    idx_t limit = n - 1;
    ensure_mu(limit);

    /* n²–1 is the largest integer < n² */
    seq_t m = (seq_t)n * (seq_t)n - 1;
    seq_t sum = 0;

    for (idx_t d = 1; d <= limit; ++d)
    {
        int8_t mu = mu_tbl[d];
        if (mu == 0)
            continue;

        seq_t term = m / ((seq_t)d * (seq_t)d);

        if (mu > 0)
            sum += term;
        else
            sum -= term;
    }

    return sum;
}
