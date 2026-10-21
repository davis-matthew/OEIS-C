/*
 * A274675  Primes p such that p = x^2 + 14*y^2 or p = 2*x^2 + 7*y^2, where p
 *          != 2, 7 and x, y are integers.
 * https://oeis.org/A274675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) computed by searching for primes p≠2,7 such that
 *          p≡1,9,15,23,25,39 (mod 56).  These are exactly the primes
 *          representable by p=x^2+14y^2 or p=2x^2+7y^2.
 *
 * The function uses the utility is_prime(idx_t) provided by
 * "../utils/is_prime.h".
 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Undefined for n < 1 – return 0 as a graceful fallback. */
    if (n < 1) return 0;

    /* Static storage for the primes that satisfy the test. */
    static seq_t *qual = NULL;
    static size_t cap  = 0;     /* Allocated capacity. */
    static size_t len  = 0;     /* Number of primes stored. */
    static idx_t last = 3;      /* Candidate to test (odd only). */

    /* If already found enough terms, return the cached value. */
    if (len >= n) return qual[n - 1];

    /* Generate primes until we have at least n qualifying terms. */
    while (len < n) {
        /* Find the next prime. */
        while (!is_prime(last)) {
            last += 2;            /* Skip even numbers. */
        }

        /* Exclude 2 and 7 and test the required residue classes. */
        if (last != 2 && last != 7) {
            idx_t r = last % 56;
            if (r == 1 || r == 9 || r == 15 || r == 23 || r == 25 || r == 39) {
                /* Ensure capacity before storing. */
                if (len == cap) {
                    size_t new_cap = cap ? cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(qual, new_cap * sizeof(seq_t));
                    if (!tmp) { /* allocation failed – abort gracefully */ }
                    qual = tmp;
                    cap = new_cap;
                }
                qual[len++] = (seq_t)last;
            }
        }

        /* Prepare next candidate (skip even numbers). */
        last += 2;
    }

    return qual[n - 1];
}
