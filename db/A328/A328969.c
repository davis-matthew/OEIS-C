/*
 * A328969  Irregular table T(n,k), n >= 2, k=1..pi(n). arising in expressing
 *          the sequence A006022 as the coefficients depending on the maximal
 *          k-th prime factor pk of the formula for A006022(n) of its unique
 *          prime factor equation.
 * https://oeis.org/A328969
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   factor_u64, pow_uint, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A328969 - Irregular table T(n,k) read row‑major.
   a(n) is obtained by concatenating the rows of the table for
   n = 2,3,4,…; the offset is 2 (a(2) is the first term). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_mul.h"  /* seq_mul   */
#include "../utils/seq_pow.h"  /* seq_pow   */
#include "../utils/pow_uint.h" /* pow_uint  */
#include "../utils/factor_u64.h"/* factor_u64 */

static bool     init_done = false;
static idx_t    max_n     = 200000;           /* sieve limit – plenty for n≈1e4 */
static uint64_t *primes   = NULL;            /* list of primes */
static size_t   prime_cnt = 0;
static idx_t   *pi_arr    = NULL;            /* pi(x) */
static idx_t   *prefix_arr= NULL;            /* cumulative lengths of rows */

/* Pre‑compute primes, pi(x) and prefix sums. */
static void
precompute(void)
{
    if (init_done) return;
    init_done = true;

    bool *is_prime = (bool*)malloc((max_n+1)*sizeof(bool));
    for (idx_t i = 0; i <= max_n; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (idx_t i = 2; i * i <= max_n; ++i)
        if (is_prime[i])
            for (idx_t j = i*i; j <= max_n; j += i)
                is_prime[j] = false;

    /* collect primes and build pi array */
    pi_arr = (idx_t*)malloc((max_n+1)*sizeof(idx_t));
    pi_arr[0] = 0;
    pi_arr[1] = 0;
    prime_cnt = 0;
    for (idx_t i = 2; i <= max_n; ++i) {
        if (is_prime[i]) prime_cnt++;
        pi_arr[i] = pi_arr[i-1] + (is_prime[i] ? 1 : 0);
    }

    /* store primes in list */
    primes = (uint64_t*)malloc(prime_cnt * sizeof(uint64_t));
    size_t pos = 0;
    for (idx_t i = 2; i <= max_n; ++i)
        if (is_prime[i]) primes[pos++] = (uint64_t)i;

    /* build prefix sums of row lengths */
    prefix_arr = (idx_t*)malloc((max_n+1)*sizeof(idx_t));
    prefix_arr[0] = prefix_arr[1] = 0;     /* no rows for n<2 */
    for (idx_t i = 2; i <= max_n; ++i)
        prefix_arr[i] = prefix_arr[i-1] + pi_arr[i];
}

/* Return the k-th prime (1‑based). */
static uint64_t
prime_k(idx_t k)
{
    /* k is guaranteed ≤ prime_cnt */
    return primes[k-1];
}

/* Main sequence routine. */
seq_t
a(idx_t n)
{
    if (n < 2) return 0;       /* undefined range – harmless */
    precompute();

    /* index shift – a(2) is at positional index 0 */
    idx_t pos = n - 2;             /* 0‑based position in concatenated table */

    /* binary search for the row */
    idx_t lo = 2, hi = max_n;
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (prefix_arr[mid] <= pos)
            lo = mid + 1;
        else
            hi = mid;
    }
    idx_t row = lo;                 /* the row containing position pos */
    idx_t prev_prefix = prefix_arr[row-1];
    idx_t col = pos - prev_prefix + 1;   /* column number, 1‑based */

    uint64_t pk = prime_k(col);

    /* If pk does not divide the row number, entry is zero. */
    if (row % pk != 0) return 0;

    /* factorise the row number */
    uint64_t fac[64];
    uint64_t exp[64];
    size_t fac_cnt = 0;
    factor_u64(row, fac, exp, &fac_cnt);

    /* find exponent of pk */
    uint64_t m_k = 0;
    for (size_t i = 0; i < fac_cnt; ++i)
        if (fac[i] == pk) { m_k = exp[i]; break; }

    /* compute denominator = product_{q <= pk, q|row} q^{exp[q]} */
    seq_t denom = 1;
    for (size_t i = 0; i < fac_cnt; ++i) {
        if (fac[i] <= pk)
            denom = denom * pow_uint(fac[i], exp[i]);
    }

    /* numerator = row / denom * (pk^m_k - 1) */
    seq_t num = ((seq_t)row) / denom;
    seq_t term = pow_uint(pk, m_k) - (seq_t)1;
    num = num * term;
    num = num / (pk - 1);  /* division is exact */

    return num;
}
