/*
 * A276050  Isolated deficient numbers that are composite.
 * https://oeis.org/A276050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1701, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: isolated deficient composite numbers (A276050) – searched via sieve. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Helper: compute sum of divisors sigma(n) using smallest prime factors (spf). */
static inline seq_t sigma_of(idx_t n, idx_t *spf)
{
    seq_t res = 1;
    idx_t temp = n;
    while (temp > 1) {
        idx_t p = spf[temp];
        int exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }
        seq_t term = 1;
        seq_t pow = 1;
        for (int e = 0; e < exp; ++e) {
            pow *= (seq_t)p;
            term += pow;
        }
        res *= term;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* sequence is 1‑indexed */

    /* static state for the sieve */
    static idx_t limit = 0;
    static idx_t *spf = NULL;

    /* initialise sieve if not already done */
    if (limit == 0) {
        limit = 200000;                   /* initial size, will grow if needed */
        spf = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
        if (!spf) return 0;
        for (idx_t i = 0; i <= limit; ++i) spf[i] = i;
        for (idx_t i = 2; i * i <= limit; ++i) {
            if (spf[i] == i) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == j) spf[j] = i;
            }
        }
    }

    idx_t count = 0;
    idx_t k = 2;                         /* start checking from 2 upwards */

    for (;;) {
        /* ensure spf covers up to k+1 for neighbor checks */
        while (k + 1 > limit) {
            idx_t new_limit = limit * 2;
            idx_t *tmp = (idx_t *)realloc(spf, (new_limit + 1) * sizeof(idx_t));
            if (!tmp) { free(spf); return 0; }
            spf = tmp;
            for (idx_t i = limit + 1; i <= new_limit; ++i) spf[i] = i;
            for (idx_t i = 2; i * i <= new_limit; ++i) {
                if (spf[i] == i) {
                    for (idx_t j = i * i; j <= new_limit; j += i)
                        if (spf[j] == j) spf[j] = i;
                }
            }
            limit = new_limit;
        }

        /* skip primes and 1 */
        if (k > 1 && spf[k] == k) {
            ++k;
            continue;
        }

        seq_t sig_k = sigma_of(k, spf);
        if (sig_k >= (seq_t)2 * (seq_t)k) { /* not deficient */
            ++k;
            continue;
        }

        seq_t sig_prev = sigma_of(k - 1, spf);
        seq_t sig_next = sigma_of(k + 1, spf);

        bool prev_def = sig_prev < (seq_t)2 * (seq_t)(k - 1);
        bool next_def = sig_next < (seq_t)2 * (seq_t)(k + 1);

        if (prev_def || next_def) {
            ++k;
            continue;
        }

        /* isolated deficient composite found */
        ++count;
        if (count == n)
            return (seq_t)k;

        ++k;
    }
}
