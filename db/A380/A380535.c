/*
 * A380535  Numbers such that the least significant nonzero digit in their
 *          primorial base representation (A049345) is greater than 1.
 * https://oeis.org/A380535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 9765, u32 n <= 9765, u64 n <= 9765,
 *            u128 n <= 9765, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) = numbers k such that in the primorial base representation
   of k the least significant non‑zero digit is greater than 1. */

#include <stdlib.h>
#include <stdbool.h>

/* Small primorials and the following primes used to build the mixed‑radix
   system. The lists are long enough for the range needed by the tests. */
static const seq_t primorials[] = {
    (seq_t)2ULL,          /* 2#                */
    (seq_t)6ULL,          /* 2·3 = 6           */
    (seq_t)30ULL,         /* 2·3·5 = 30        */
    (seq_t)210ULL,        /* 2·3·5·7 = 210     */
    (seq_t)2310ULL,       /* 2·3·5·7·11 = 2310 */
    (seq_t)30030ULL,      /* 2·3·5·7·11·13      */
    (seq_t)510510ULL,     /* 2·3·5·7·11·13·17 */
    (seq_t)9699690ULL,    /* times 19          */
    (seq_t)223092870ULL,  /* times 23          */
    (seq_t)6469693230ULL, /* times 29          */
    (seq_t)200560490130ULL, /* times 31       */
    (seq_t)7420738134810ULL  /* times 37      */
};
static const idx_t primes[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL,
    19ULL, 23ULL, 29ULL, 31ULL, 37ULL
};
static const size_t PCOUNT = sizeof(primorials)/sizeof(primorials[0]);

/* Return true iff the first non‑zero digit in the primorial base expansion
   of k is greater than 1. */
static bool test_condition(idx_t k)
{
    seq_t x = (seq_t)k;
    /* Least significant digit (a0) must be 0 (i.e. k must be even). */
    if ((x & (seq_t)1) != (seq_t)0)
        return false;

    /* Scan higher digits a1, a2, … for the first non‑zero. */
    for (size_t i = 0; i < PCOUNT; ++i) {
        seq_t P = primorials[i];
        seq_t coeff = (x / P) % primes[i + 1];
        if (coeff != (seq_t)0)           /* found least significant non‑zero digit */
            return coeff > (seq_t)1;     /* check if it exceeds 1 */
    }

    /* In the unlikely event that all checked digits are zero (k=0), return false. */
    return false;
}

seq_t a(idx_t n)
{
    /* Cache of computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of stored terms */
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t next_candidate = 1; /* next integer to test */

    while (cache_len < n) {
        /* Find next integer satisfying the condition. */
        while (!test_condition(next_candidate))
            ++next_candidate;

        /* Append to cache. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                exit(1); /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }

    return cache[n - 1];
}
