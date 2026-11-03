/*
 * A278254  Least number with the prime signature of n^2; square of the least
 *          number with the prime signature of n.
 * https://oeis.org/A278254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9990 (2026-11-01)
 * Helpers:   factor_u64, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = A046523(n)^2
 * where A046523(n) is the least number with the prime signature of n.
 *
 * We compute a(n) as the least number with the prime signature of n²,
 * which equals the square of the least number with the prime signature of n.
 *
 * Implementation steps:
 *   1. Factor n into primes and exponents using factor_u64.
 *   2. Sort the exponents in descending order.
 *   3. Multiply the corresponding smallest primes raised to those exponents.
 *   4. Square the resulting product.
 *
 * The function is O(k log k) with k = number of prime factors of n,
 * and works for any n >= 1 on the supported integer sizes.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Required helper functions provided by the build environment */
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_u64_idx.h"

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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* insertion sort for a small array, sorting in descending order */
static void sort_descending(int *arr, size_t n)
{
    for (size_t i = 1; i < n; ++i) {
        int key = arr[i];
        size_t j = i;
        while (j > 0 && arr[j - 1] < key) {   /* descending */
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = key;
    }
}

/* Sort an array of exponents in descending order (simple insertion sort) */

/* Sequence generator */
seq_t a(idx_t n)
{
    /* Handle the trivial case n = 0 if called (not part of the OEIS offset) */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Special case for n = 1 */
    if (n == 1) {
        return (seq_t)1;
    }

    /* Arrays to hold prime factors and exponents */
    uint64_t primes[64];
    uint64_t exps_u64[64];
    size_t factor_cnt = 0;

    /* Factor n */
    factor_u64((uint64_t)n, primes, exps_u64, &factor_cnt);
    if (factor_cnt == 0) {            /* this can only happen for n = 1, already handled */
        return (seq_t)1;
    }

    /* Convert exponents to int for sorting */
    int exps[64];
    for (size_t i = 0; i < factor_cnt; ++i) {
        exps[i] = (int)exps_u64[i];
    }

    /* Sort exponents descending so the largest exponent gets the smallest prime */
    sort_descending(exps, factor_cnt);

    /* Compute the least number with the same prime signature */
    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < factor_cnt; ++i) {
        uint32_t prime = get_prime((idx_t)(i + 1));   /* primes are 2,3,5,... */
        seq_t term   = ipow_u64_idx((uint64_t)prime, (idx_t)exps[i]);
        prod         = seq_mul(prod, term);
    }

    /* Square the result to obtain a(n) = A046523(n)^2 */
    return seq_mul(prod, prod);
}
