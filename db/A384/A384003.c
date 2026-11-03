/*
 * A384003  Irregular triangle T(n,k), n >= 0, 0 <= k < 2^(n-1), where T(n,k)
 *          = Product_{j=0..n-1} prime(j+1)^((n-j)*d_j), where d_j is the bit
 *          with digit weight 2^j in the binary expansion of 2^(n-1)+k.
 * https://oeis.org/A384003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 46, u64 n <= 158, u128 n <= 766,
 *            big any n
 * Validated: standard, pass, 59 terms through n = 7981 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A384003 – Irregular Triangle
 *   a(0) = 1
 *   For n >= 1 let r = ⌊log₂ n⌋ + 1 (row number) and k = n – 2^{r‑1}
 *   Then a(n) = T(r,k) = ∏_{j=0}^{r‑1} prime(j+1)^{(r‑j)·bit_j(n)},
 *   where bit_j(n) is the j-th bit of the binary representation of n
 *   (within 0 ≤ j < r).  The top bit j = r‑1 is always 1, so prime(r)
 *   always appears with exponent 1.
 *
 * This implementation uses the helper functions
 *   get_prime(idx_t k)          – returns the k‑th prime (k ≥ 1)
 *   seq_pow(seq_t base, idx_t exp) – computes base^exp as seq_t
 *
 * The routine runs in O(log n) time per call. */

#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>
#include <stddef.h>
#include "../utils/ipow_idx.h"

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

/* Compute ⌊log₂ v⌋ for a non‑zero 64‑bit integer. */
static inline unsigned floor_log2_uint64(uint64_t v)
{
    unsigned r = 0;
    while (v > 1) {
        v >>= 1;
        ++r;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Handle the special case of the very first term. */
    if (n == 0)
        return (seq_t)1;

    /* Determine the row (r) and column (k). */
    unsigned r = floor_log2_uint64(n) + 1;        /* r ≥ 1 */
    /* start = 2^(r-1) is also the first index of this row */
    idx_t start = 1ULL << (r - 1);

    /* The value that appears in the bit pattern is simply n itself
     * (since n = 2^(r-1) + k for all n ≥ 1). */
    idx_t val = n;

    seq_t result = (seq_t)1;

    /* Iterate over all bit positions j = 0 .. r‑1. */
    for (unsigned j = 0; j < r; ++j) {
        /* Check whether bit j of val is set. */
        if (val & (1ULL << j)) {
            /* Exponent for this prime = r - j. */
            idx_t exp = (idx_t)(r - j);

            /* prime(j+1) is the (j+1)-th prime. */
            seq_t prime = (seq_t)get_prime(j + 1);

            /* Multiply into the running product. */
            result *= ipow_idx(prime, exp);
        }
    }

    return result;
}
