/*
 * A255274  From Goldbach conjecture: Consider the pairs (2n-+1, 3), (2n-1,
 *          5), (2n-3, 7), ..., (3, 2n+1) of odd numbers having sum 2n+4; a(n)
 *          is the index of the first pair of primes (p, q) on the list.
 * https://oeis.org/A255274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest i (1‑based) such that both * 2*n+1–2*(i‑1)
 *            and 2*(i‑1)+3 are prime; i ranges 1…n. * If no such pair exists
 *            (theoretically impossible) return n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static bool *prime_is = NULL;   /* sieve array */
static idx_t prime_limit = 0;   /* maximum number currently sieved */

/* Ensure that primes up to 'limit' are available. Rebuild the full sieve
 * whenever the limit grows, which is adequate for n up to about 10 000. */
static void
ensure_prime_limit(idx_t limit)
{
    if (limit <= prime_limit) return;

    idx_t new_limit = (prime_limit == 0) ? 4 : prime_limit * 2;
    while (new_limit < limit) new_limit <<= 1;   /* double until large enough */

    bool *new_arr = (bool*)malloc((new_limit + 1) * sizeof(bool));
    if (!new_arr) abort();                       /* out of memory */

    /* Initialize all as prime, then sieve */
    for (idx_t i = 0; i <= new_limit; ++i) new_arr[i] = true;
    new_arr[0] = new_arr[1] = false;

    for (idx_t i = 2; i * i <= new_limit; ++i) {
        if (new_arr[i]) {
            for (idx_t j = i * i; j <= new_limit; j += i)
                new_arr[j] = false;
        }
    }

    free(prime_is);
    prime_is = new_arr;
    prime_limit = new_limit;
}

seq_t
a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n=0, guard */

    idx_t needed = 2 * n + 1;              /* largest number in the pairs */
    ensure_prime_limit(needed);

    for (idx_t k = 0; k < n; ++k) {        /* k = 0 corresponds to i = 1 */
        idx_t p = 2 * n + 1 - 2 * k;       /* 2n+1–2k   */
        idx_t q = 2 * k + 3;               /* 2k+3      */
        if (prime_is[p] && prime_is[q])
            return (seq_t)(k + 1);         /* 1‑based index */
    }

    /* Fallback – should never be reached under Goldbach's conjecture. */
    return (seq_t)n;
}
