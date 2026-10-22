/*
 * A367018  Composite squarefree k that are not divisible by 6.
 * https://oeis.org/A367018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A367018: composite squarefree numbers not divisible by 6, offset 1.
   The implementation builds a sieve up to a fixed limit, stores the
   qualifying values in a static array and returns the nth entry.  It works
   for any seq_t, including arbitrary‑precision implementations.  The
   sieve limit is chosen large enough that the first 10 000 terms are
   precomputed (the density of this sequence is 1/(2π²) ≈ 0.05066). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static state persists between calls. */
    static bool initialized = false;
    static idx_t *spf = NULL;               /* smallest prime factor array   */
    static idx_t sieve_limit = 0;           /* current sieve upper bound     */
    static seq_t *list = NULL;              /* sequence values               */
    static size_t list_cap = 0;             /* capacity of the array         */
    static size_t list_len = 0;             /* number of filled entries      */

    if (!initialized) {
        /* choose a sieve limit that covers the first ~10 000 terms. */
        sieve_limit = 2000000;              /* 2·10⁶ is ample for n ≈ 10 000 */

        /* Allocate and build the smallest‑prime‑factor table. */
        spf = (idx_t *)malloc((sieve_limit + 1) * sizeof(idx_t));
        for (idx_t i = 0; i <= sieve_limit; ++i) spf[i] = i;
        for (idx_t i = 2; i * i <= sieve_limit; ++i) {
            if (spf[i] == i) {             /* i is prime */
                for (idx_t j = i * i; j <= sieve_limit; j += i)
                    if (spf[j] == j) spf[j] = i;
            }
        }

        /* Allocate a dynamic array for the sequence values. */
        list_cap = 1024;
        list = (seq_t *)malloc(list_cap * sizeof(seq_t));
        list_len = 0;

        /* Generate the sequence up to sieve_limit. */
        for (idx_t val = 2; val <= sieve_limit; ++val) {
            /* Skip primes. */
            if (spf[val] == val) continue;

            /* Skip numbers divisible by 6. */
            if ((val % 6) == 0) continue;

            /* Check square‑free by inspecting prime factor exponents. */
            idx_t y = val;
            bool squarefree = true;
            while (y > 1) {
                idx_t p = spf[y];
                int cnt = 0;
                while (y % p == 0) {
                    y /= p;
                    ++cnt;
                }
                if (cnt > 1) {              /* repeated factor → not square‑free */
                    squarefree = false;
                    break;
                }
            }
            if (!squarefree) continue;

            /* Store the value; grow the array if necessary. */
            if (list_len == list_cap) {
                list_cap <<= 1;             /* double the capacity */
                list = (seq_t *)realloc(list, list_cap * sizeof(seq_t));
            }
            list[list_len++] = (seq_t)val;
        }

        /* The sieve table is no longer needed. */
        free(spf);
        spf = NULL;

        initialized = true;
    }

    if (n == 0 || n > list_len)
        return 0;                         /* out of range */

    return list[n - 1];                    /* a(1) corresponds to list[0] */
}
