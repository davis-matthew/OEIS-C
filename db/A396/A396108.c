/*
 * A396108  Expansion of e.g.f. 1/(1 - sinh(x) - sinh(x)^2).
 * https://oeis.org/A396108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   binomial, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A396108: a(n) = Sum_{k=0..⌊n/2⌋} (n-2k)! * F_{n-2k+1} * A136630(n,n-2k)
   Recurrence used for fast computation:
   a(0) = 1
   a(n) = Σ_{k=0..⌊(n-1)/2⌋} binomial(n,2k+1) * a(n-2k-1)
          + 2 * Σ_{k=1..⌊n/2⌋} 4^{k-1} * binomial(n,2k) * a(n-2k) */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t a, idx_t b) */
#include "../utils/pow4.h"       /* seq_t pow4(idx_t exp) */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of computed a(i) values */
    static size_t cap   = 0;      /* capacity of cache array          */
    static idx_t last_computed = (idx_t)-1; /* highest index computed */

    /* Initialize cache if empty */
    if (cap == 0) {
        cap = 1;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        last_computed = 0;
    }

    /* Grow cache if needed */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)(n + 1))
            new_cap <<= 1;

        seq_t *tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
        for (size_t i = 0; i < cap; ++i)
            tmp[i] = cache[i];
        free(cache);
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute new entries up to n */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
                continue;
            }

            /* Sum over odd binomial terms */
            seq_t sum1 = 0;
            idx_t maxk1 = (i - 1) / 2;
            for (idx_t k = 0; k <= maxk1; ++k) {
                seq_t term = binomial(i, 2 * k + 1);
                term *= cache[i - 2 * k - 1];
                sum1 += term;
            }

            /* Sum over even binomial terms */
            seq_t sum2 = 0;
            idx_t maxk2 = i / 2;
            for (idx_t k = 1; k <= maxk2; ++k) {
                seq_t term = pow4(k - 1);                 /* 4^(k-1)      */
                term *= binomial(i, 2 * k);               /* binomial(n,2k)*/
                term *= cache[i - 2 * k];                 /* a(n-2k)      */
                sum2 += term;
            }

            cache[i] = sum1 + (sum2 << 1);   /* 2 * sum2      */
        }
        last_computed = n;
    }

    return cache[n];
}
