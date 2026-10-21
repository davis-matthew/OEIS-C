/*
 * A261371  Prime numbers that sum to a prime when added to their digital
 *          root.
 * https://oeis.org/A261371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A261371: primes p such that p + digital_root(p) is prime. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

static seq_t *prop_primes = NULL;
static size_t prop_cap   = 0;
static size_t prop_cnt   = 0;
static seq_t   cand      = 2;

/* Ensure enough capacity for the property prime list */
static void grow_prop(void) {
    size_t new_cap = prop_cap == 0 ? 16 : prop_cap << 1;
    seq_t *tmp = (seq_t *)realloc(prop_primes, new_cap * sizeof(seq_t));
    if (!tmp) abort();            /* allocation failed */
    prop_primes = tmp;
    prop_cap    = new_cap;
}

/* Find the next prime p such that p + digital_root(p) is prime
   and store it in prop_primes.  cand holds the next candidate
   to test. */
static void find_next_property_prime(void) {
    for (;;) {
        seq_t p;
        /* Find the next prime starting from cand */
        if (cand <= 2) {
            p = 2;
            cand = 3;                 /* next candidate will be 3 (odd) */
        } else {
            p = cand;
            if ((p & 1) == 0) p += 1; /* ensure we start with an odd number */
            while (!is_prime_big(p)) {
                p += 2;               /* skip even numbers */
            }
            cand = p + 2;             /* next trial starts after this prime */
        }

        /* Compute digital root: 1 + ((p-1) % 9) */
        seq_t dr  = ((p - 1) % 9) + 1;
        seq_t sum = p + dr;
        if (is_prime_big(sum)) {
            if (prop_cnt >= prop_cap) grow_prop();
            prop_primes[prop_cnt++] = p;
            break;                    /* we found a property prime */
        }
        /* otherwise continue searching for the next prime */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* undefined for n < 1 */
    while (prop_cnt < (size_t)n) {
        find_next_property_prime();        /* generate more terms as needed */
    }
    return prop_primes[(size_t)n - 1];
}
