/*
 * A258566  Triangle in which n-th row contains all possible products of n-1
 *          of the first n primes in descending order.
 * https://oeis.org/A258566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (Π_{i=1}^{r} p_i) / p_k * where r is the row such that
 *            r(r‑1)/2 < n ≤ r(r+1)/2 * and k = n – r(r‑1)/2. p_i denotes the
 *            i‑th prime
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

/* Cached cumulative prime products, 1‑based. */
static seq_t *prod = NULL;
static idx_t prod_cap = 0;

/* Ensure prod[1..r] are initialized. */
static void ensure_prod(idx_t r)
{
    if (prod_cap >= r)
        return;
    size_t new_cap = (size_t)(r + 1);             /* index 0 unused */
    seq_t *tmp = mem_resize(seq_t, prod, new_cap);
    if (!tmp)                                   /* fatal if memory exhausted */
        exit(1);
    prod = tmp;
    for (idx_t i = prod_cap + 1; i <= r; ++i) {
        seq_t p = (seq_t)get_prime(i);
        if (i == 1)
            prod[1] = p;
        else
            prod[i] = prod[i - 1] * p;
    }
    prod_cap = r;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;    /* sequence offset 1 */

    /* Find the row r: r(r+1)/2 ≥ n. */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;

    /* Position k within the row (1‑based). */
    idx_t prev_sum = (r - 1) * r / 2;
    idx_t k = n - prev_sum;

    ensure_prod(r);

    return prod[r] / (seq_t)get_prime(k);
}
