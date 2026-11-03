/*
 * A279889  a(n) = Sum_{k=1..n-1} sigma_5(k)*sigma_5(n-k).
 * https://oeis.org/A279889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n-1} sigma_5(k) * sigma_5(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 15, u64 n <= 115, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 901 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                  /* for size_t */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_cache(seq_t *p)
{
    mem_free(p);
}

seq_t a(idx_t n)
{
    /* a(1) is the first term; for n <= 1 the sum is empty */
    if (n <= 1)
        return (seq_t)0;

    /* cache for sigma_5 values up to the current n-1 */
    static seq_t *sigma5 = NULL;
    static idx_t cap = 0;

    /* enlarge cache if necessary */
    if (n > cap) {
        free_cache(sigma5);                       /* safe even if NULL */
        sigma5 = alloc_seq_array((size_t)n);      /* allocate array of seq_t */
        cap = n;
    }

    /* compute sigma_5(k) for k = 1 .. n-1 */
    for (idx_t k = 1; k < n; ++k) {
        sigma5[k] = sigma_k(k, (uint8_t)5);
    }

    /* convolution sum */
    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k < n; ++k) {
        sum += sigma5[k] * sigma5[n - k];
    }

    return sum;
}
