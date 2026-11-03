/*
 * A336360  Numbers k for which bigomega(sigma(k)) >= bigomega(k), where
 *          bigomega (A001222) gives the number of prime factors with
 *          multiplicity, and sigma (A000203) gives the sum of divisors.
 * https://oeis.org/A336360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Validated: partial, pass, 82 terms through n = 82 (2026-11-03)
 * Helpers:   bigomega, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A336360: numbers k such that bigomega(σ(k)) ≥ bigomega(k)
   (computed by forward search; cache is kept across calls). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/bigomega.h"
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n) {
    /* offset 1: a(1) = 1 */
    static seq_t *cache = NULL;   /* cached sequence values   */
    static size_t cap   = 0;      /* allocated capacity in entries   */
    static size_t count = 0;      /* number of entries cached        */
    static idx_t  next   = 1;     /* next integer k to test          */

    if (n == 0) return 0;          /* undefined for n < 1, return 0 */

    /* ensure enough storage for first n terms */
    if (cap < n) {
        size_t new_cap = cap ? cap * 2 : 64;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();        /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* extend the cache until we have at least n terms */
    while (count < n) {
        seq_t sigma = sigma_of(next);                    /* σ(next)            */
        seq_t omega_sigma = bigomega((idx_t)sigma);      /* Ω(σ(next))         */
        seq_t omega_k     = bigomega(next);              /* Ω(next)            */
        if (omega_sigma >= omega_k) {
            cache[count++] = (seq_t)next;                /* store valid k      */
        }
        ++next;                                          /* move to next integer */
    }

    return cache[n - 1];                                 /* nth term */
}
