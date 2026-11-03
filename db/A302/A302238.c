/*
 * A302238  Expansion of Product_{k>=1} ((1 + x^k)/(1 - x^k))^prime(k).
 * https://oeis.org/A302238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * sum_{k=1}^{n} ( 2 * sum_{d|k, (k/d) odd} d * p_d
 *            ) * a(n-k) where p_d is the d-th prime (A000040(d)). This
 *            recurrence follows from the logarithm of the product G(x) =
 *            product_{k>=1} ((1 + x^k)/(1 - x^k))^{p_k}
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 30 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    /* allocate array for S(k) = 2 * sum_{d|k , odd m = k/d} d * p_d  (uint64_t) */
    uint64_t *S = (uint64_t*)calloc((size_t)(n + 1), sizeof(uint64_t));
    if (!S) return (seq_t)0;        /* fallback on allocation failure */

    /* compute S(k) */
    for (idx_t d = 1; d <= n; ++d) {
        uint64_t pd = get_prime(d);          /* p_d  (32‑bit)  */
        uint64_t val = 2ULL * (uint64_t)d * pd;   /* 2 * d * p_d  */
        for (uint64_t e = 1; d * e <= n; e += 2) { /* e odd */
            idx_t idx = d * e;
            S[idx] += val;
        }
    }

    /* dynamic programming for a(0..n) */
    seq_t *a = mem_new(seq_t, (size_t)(n + 1));
    if (!a) { free(S); return (seq_t)0; }

    a[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t prod = a[i - k] * (seq_t)S[k];
            sum += prod;
        }
        a[i] = sum / (seq_t)i;        /* division is exact */
    }

    seq_t res = a[n];
    free(S);
    mem_free(a);
    return res;
}
