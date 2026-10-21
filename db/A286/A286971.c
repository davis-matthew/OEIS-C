/*
 * A286971  Number of ways to write n as a sum of two numbers, one of which is
 *          the product of an even number of distinct primes (including 1)
 *          (A030229) and another is the product of an odd number of distinct
 *          primes (A030059).
 * https://oeis.org/A286971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered pairs (x , y) with x ∈ A030229
 *            (product of an even number of distinct primes, including 1), y ∈
 *            A030059 (product of an odd number of distinct primes), and x + y
 *            = n. The sets are disjoint, so each pair is counted once
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_spf.h"

/* Global state – memoised for all calls. */
static idx_t seen_max = 0;          /* largest n that has been classified */
static idx_t spf_limit = 0;         /* largest n for which an SPF array exists */
static idx_t *spf = NULL;           /* smallest prime factor array */
static unsigned char *even_set = NULL; /* 1 if i ∈ A030229, else 0 */
static unsigned char *odd_set  = NULL; /* 1 if i ∈ A030059, else 0 */

/* Ensure that all numbers up to nmax are classified into A030229 and A030059. */
static void
ensure_arrays(idx_t nmax)
{
    if (nmax <= seen_max)
        return;                     /* already done */

    /* 1. Build or extend the smallest‑prime‑factor table if needed. */
    if (nmax > spf_limit) {
        build_spf(nmax, &spf);
        spf_limit = nmax;
    }

    /* 2. Grow the classification tables. */
    unsigned char *new_even = (unsigned char *)realloc(even_set,
                                                      (size_t)(nmax + 1));
    unsigned char *new_odd  = (unsigned char *)realloc(odd_set,
                                                      (size_t)(nmax + 1));
    if (!new_even || !new_odd) {
        abort();                  /* out of memory – acceptable */
    }
    even_set = new_even;
    odd_set  = new_odd;

    /* 3. Initialise new entries to zero. */
    for (idx_t i = seen_max + 1; i <= nmax; ++i) {
        even_set[i] = 0;
        odd_set[i]  = 0;
    }

    /* 4. Classify each number from seen_max+1 to nmax. */
    for (idx_t i = seen_max + 1; i <= nmax; ++i) {
        idx_t k = i;
        int   distinct = 0;      /* number of distinct prime factors */
        int   squarefree = 1;    /* become 0 if a square factor is found */

        while (k > 1) {
            idx_t p = spf[k];
            ++distinct;

            k /= p;
            while (k % p == 0) {   /* repeated prime factor → not squarefree */
                squarefree = 0;
                k /= p;
            }
        }

        if (squarefree) {
            if ((distinct & 1) == 0) {  /* even number of primes (A030229) */
                even_set[i] = 1;
                odd_set[i]  = 0;
            } else {                    /* odd number of primes (A030059) */
                even_set[i] = 0;
                odd_set[i]  = 1;
            }
        }
    }

    seen_max = nmax;
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    if (n < 2)                     /* a(0)=0, a(1)=0 */
        return (seq_t)0;

    ensure_arrays(n);

    seq_t count = 0;
    /* Loop over all x in A030229;  y = n - x automatically lies in A030059. */
    for (idx_t x = 1; x <= n - 1; ++x) {
        if (even_set[x] && odd_set[n - x])
            ++count;
    }
    return count;
}
