/*
 * A356342  a(n) = Sum_{k=1..n} binomial(2*n, k) * sigma_2(k).
 * https://oeis.org/A356342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(2*n, k) * sigma_2(k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 23 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t sigma_k(idx_t m, uint8_t k)
{
    seq_t res = (seq_t)1;
    idx_t n = m;

    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            idx_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }

            /* compute pk = p^k */
            seq_t pk = (seq_t)1;
            for (idx_t i = 0; i < k; ++i) pk *= (seq_t)p;

            /* sum_{i=0}^{exp} (p^k)^i = 1 + pk + pk^2 + ...   */
            seq_t term = (seq_t)0;
            seq_t pow   = (seq_t)1;
            for (idx_t i = 0; i <= exp; ++i) {
                term += pow;
                pow  *= pk;
            }
            res *= term;
        }
    }

    if (n > 1) {               /* remaining prime factor with exponent 1 */
        /* k is 1 or 3, exp = 1 */
        seq_t pk = (seq_t)1;
        for (idx_t i = 0; i < k; ++i) pk *= (seq_t)n;

        seq_t term = (seq_t)0;
        seq_t pow   = (seq_t)1;
        for (idx_t i = 0; i <= (idx_t)1; ++i) {
            term += pow;
            pow  *= pk;
        }
        res *= term;
    }

    return res;
}

/* -------------------------------------------------------------- */
/* Cache for sigma_2(k) values. 1‑based array.                   */
/* -------------------------------------------------------------- */
static seq_t *sigma_cache = NULL;   /* indexed from 1 */
static idx_t sigma_cap   = 0;       /* current capacity (indices 0..sigma_cap inclusive) */

static void ensure_sigma_cap(idx_t need)
{
    if (need <= sigma_cap)
        return;                /* already cached */

    /* Allocate space for indices 0..need */
    idx_t new_cap = need;
    seq_t *tmp = mem_resize(seq_t, sigma_cache, (new_cap + 1));
    assert(tmp != NULL);       /* allocation failure: abort */

    /* Initialize unused 0th element (values are 0-based unused) */
    tmp[0] = 0;

    /* Compute sigma_2 for the new indices */
    for (idx_t i = sigma_cap + 1; i <= new_cap; ++i)
        tmp[i] = sigma_k(i, 2);

    sigma_cache = tmp;
    sigma_cap   = new_cap;
}

/* -------------------------------------------------------------- */
/* Sequence a(n) as defined by the OEIS entry                   */
/* -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)          /* offset starts at 1; undefined below */
        return 0;

    ensure_sigma_cap(n);          /* make sure sigma_cache[1..n] exist */

    seq_t sum   = 0;               /* running total of the sum */
    idx_t two_n = n << 1;          /* 2 * n as idx_t */
    seq_t binom = 1;               /* C(2n,0) */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update binom to C(2n,k) using the recurrence */
        binom = (binom * (seq_t)(two_n - k + 1)) / (seq_t)k;

        /* Add contribution C(2n,k) * sigma_2(k) to the sum */
        sum += binom * sigma_cache[k];
    }

    return sum;
}
