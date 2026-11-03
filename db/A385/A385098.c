/*
 * A385098  Numbers k such that Moebius(espf(k)) > 0, where espf(k) is the
 *          exponent of the smallest prime factor of k, with espf(1) = 1.
 * https://oeis.org/A385098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A385098: the nth positive integer k such that μ(exp_smallest_prime_factor(k)) = 1.
   We generate the terms incrementally by testing each integer k.
   For k = 1 the exponent is defined to be 1; μ(1) = 1, so 1 is the first term.
   For k > 1, we find its smallest prime factor p and the exponent e of p in k.
   If μ(e) == 1 (i.e. e is square‑free with an even number of distinct prime factors,
   or e == 1) then k is stored as a term.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return 1 for μ(1), 1 for square‑free e with an even number of prime factors,
   -1 for square‑free e with an odd number of prime factors, and 0 otherwise. */
static int mu_of_uint64(uint64_t n)
{
    if (n == 1)
        return 1;
    int distinct = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int cnt = 0;
            while (n % p == 0) {
                n /= p;
                ++cnt;
                if (cnt > 1)
                    return 0;                       /* not square‑free */
            }
            ++distinct;
        }
    }
    if (n > 1)
        ++distinct;
    return (distinct % 2 == 0) ? 1 : -1;
}

/* Compute the exponent of the smallest prime factor of k.
   espf(1) is defined to be 1. */
static uint64_t smallest_prime_factor_exponent(idx_t k)
{
    if (k == 1)
        return 1;                         /* special definition */
    idx_t n = k;
    idx_t p;

    if (n % 2 == 0) {
        p = 2;
    } else {
        for (p = 3; (idx_t)p * p <= n; p += 2) {
            if (n % p == 0)
                break;
        }
        if ((idx_t)p * p > n)                 /* n is prime */
            p = n;
    }

    uint64_t e = 0;
    while (n % p == 0) {
        n /= p;
        ++e;
    }
    return e;
}

/* Main sequence function requested by the build system. */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;  /* stored terms a(1), a(2), … */
    static size_t cap   = 0;     /* allocated capacity of 'terms'      */
    static size_t sz    = 0;     /* number of terms currently stored   */
    static idx_t last_k = 0;     /* last integer examined              */

    /* Expand storage until we have at least n terms. */
    while (sz < (size_t)n) {
        ++last_k;                         /* next candidate k                */
        uint64_t e = smallest_prime_factor_exponent(last_k);

        /* μ(e) > 0 ⇔ μ(e) == 1 */
        if (mu_of_uint64(e) == 1) {
            if (sz == cap) {             /* grow array if necessary        */
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, terms, new_cap);
                if (!tmp) abort();       /* unrecoverable error            */
                terms = tmp;
                cap = new_cap;
            }
            terms[sz++] = (seq_t)last_k;
        }
    }
    return terms[n - 1];
}
