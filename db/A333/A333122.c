/*
 * A333122  Numbers m such that m = prime(k) + prime(k+5) = prime(k+1) +
 *          prime(k+4) for some k.
 * https://oeis.org/A333122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 430, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9318 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A333122: a(n) = number m such that
   prime(k)+prime(k+5) = prime(k+1)+prime(k+4)
   for some k.  The terms are generated in increasing
   order by increasing k.  The function is memoised.
 */

#include <stdlib.h>
#include <stdint.h>
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

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

static seq_t *terms = NULL;     /* array of found terms          */
static size_t terms_cap = 0;    /* current capacity of 'terms'   */
static size_t terms_len = 0;    /* number of terms stored        */
static idx_t next_k = 3;        /* next k to test (starting at 3) */

/* Ensure that terms[0..needed-1] are computed. */
static void ensure_terms(size_t needed)
{
    if (terms_len >= needed)
        return;

    /* enlarge storage as needed */
    grow_seq(&terms, &terms_cap, needed);

    /* generate terms until we have enough */
    while (terms_len < needed) {
        /* fetch primes needed for current k (indices are 1‑based) */
        seq_t pk   = (seq_t)get_prime(next_k);
        seq_t pk1  = (seq_t)get_prime(next_k + 1);
        seq_t pk4  = (seq_t)get_prime(next_k + 4);
        seq_t pk5  = (seq_t)get_prime(next_k + 5);

        seq_t s1 = pk + pk5;   /* prime(k) + prime(k+5)  */
        seq_t s2 = pk1 + pk4;  /* prime(k+1) + prime(k+4) */

        if (s1 == s2)
            terms[terms_len++] = s1;

        ++next_k;
    }
}

seq_t a(idx_t n)
{
    /* n is 1‑based; ignore calls with n==0 for safety */
    if (n == 0)
        return 0;

    ensure_terms((size_t)n);
    return terms[n - 1];
}
