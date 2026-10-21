/*
 * A246009  Length of Collatz cycles '3*n + 1' of prime numbers.
 * https://oeis.org/A246009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the length of the Collatz sequence starting from prime(n)
 *            * (including both endpoints)
 * Range:     u8 n <= 10, u16 n <= 2646, u32 n <= 8922, u64 n <= 8922,
 *            u128 n <= 8922, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

/* Helper to build at least the first `need` primes (0‑based count).
   The primes are stored in a static array of type seq_t. */
static seq_t *prime_cache = NULL; /* array of primes */
static size_t prime_cap   = 0;     /* current allocated capacity */
static idx_t  prime_cnt   = 0;     /* number of primes stored */

/* Expand the prime cache until it contains at least `need` primes. */
static void
ensure_primes(idx_t need)
{
    if (prime_cnt >= need)
        return;

    /* Rough upper bound for the nth prime (for n>=6):
       n (log n + log log n) + 3.  */
    double n = (double)need;
    double limit = (need >= 6) ? n * (log(n) + log(log(n))) + 3.0
                               : 15.0;

    while (prime_cnt < need) {
        size_t lim = (size_t)limit;
        bool *is_comp = (bool *)calloc(lim + 1, sizeof(bool));
        if (!is_comp)
            return; /* memory failure – caller will get whatever it has */

        size_t count = 0;
        for (size_t i = 2; i <= lim; ++i) {
            if (!is_comp[i]) {
                ++count;
                if (count > prime_cnt) { /* store new prime */
                    if (count > prime_cap) {
                        prime_cap = count * 2;
                        prime_cache = (seq_t *)realloc(prime_cache,
                                                       prime_cap * sizeof(seq_t));
                        if (!prime_cache) {
                            free(is_comp);
                            return;
                        }
                    }
                    prime_cache[count - 1] = (seq_t)i;
                }
                /* Mark multiples */
                if (i * (uint64_t)i <= lim) {
                    for (size_t j = i * i; j <= lim; j += i)
                        is_comp[j] = true;
                }
            }
        }
        free(is_comp);
        prime_cnt = count;
        if (prime_cnt >= need)
            return;
        limit *= 2.0; /* increase limit if estimate was too low */
    }
}

/* Return the a(n) value for OEIS A246009. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n < 1, but return 0 */

    /* Ensure we already have the nth prime (n is 1‑based). */
    ensure_primes(n);
    seq_t start = prime_cache[n - 1];
    seq_t steps = 1;           /* count the start term */
    seq_t x = start;

    /* Run the Collatz 3x+1 map until we hit 1. */
    while (x != 1) {
        if ((x & (seq_t)1) == 0)        /* even */
            x = x >> 1;                 /* divide by 2 */
        else                            /* odd */
            x = x * (seq_t)3 + (seq_t)1;
        ++steps;
    }
    return steps;
}
