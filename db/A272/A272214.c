/*
 * A272214  Square array read by antidiagonals upwards in which T(n,k) is the
 *          product of the n-th prime and the sum of the divisors of k, n >=
 *          1, k >= 1.
 * https://oeis.org/A272214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 8128, u32 n <= 8128, u64 n <= 8128,
 *            u128 n <= 8128, big any n
 * Validated: standard, pass, 94 terms through n = 7901 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A272214: square array read by antidiagonals upwards.
   Term a(n) is the product of the i-th prime and the sum-of-divisors
   of j where (i,j) are obtained from n by the antidiagonal mapping:
   1) Find the smallest d ≥ 2 such that d(d−1)/2 ≥ n.
   2) Let prev = (d−2)(d−1)/2 (number of terms before diagonal d).
   3) offset = n − prev − 1   (0‑based index inside diagonal d).
   4) i = (d−1) − offset,   j = d − i.
   5) a(n) = prime(i) * sigma(j).                               */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

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

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence starts at n = 1 */

    /* locate diagonal d */
    idx_t d = 2;
    while ((d * (d - 1)) / 2 < n) d++;

    /* number of terms before diagonal d */
    idx_t prev_total = ((d - 2) * (d - 1)) / 2;

    /* offset within diagonal d (0‑based) */
    idx_t offset = n - prev_total - 1;

    /* coordinates (i,j) */
    idx_t i = (d - 1) - offset;
    idx_t j = d - i;

    /* compute prime(i) and sigma(j) */
    seq_t prime_i = (seq_t)get_prime(i);
    seq_t sigma_j = sigma_of(j);

    /* product */
    return prime_i * sigma_j;
}
