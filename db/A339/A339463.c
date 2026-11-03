/*
 * A339463  Primes p such that (p-1)/gpf(p-1) = 2^q * 5^r with q, r >= 1,
 *          where gpf(m) is the greatest prime factor of m, A006530.
 * https://oeis.org/A339463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th prime p such that (p-1)/gpf(p-1) is a product
 *            of powers of 2 and 5, each with exponent ≥1
 * Range:     u8 n <= 5, u16 n <= 453, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 9877 (2026-11-03)
 * Helpers:   largest_prime_factor, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/largest_prime_factor.h"
#include "../utils/next_prime.h"
#include "../utils/mem.h"

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

static seq_t *terms = NULL;      /* stored terms */
static size_t   len   = 0;        /* number of stored terms   */
static size_t   cap   = 0;        /* allocated capacity      */
static uint64_t next_start = 70;  /* start below first candidate (71) */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n < 1 */

    while (len < (size_t)n) {
        /* find next prime >= next_start+1 (next_prime returns > argument) */
        uint64_t p = next_prime(next_start);
        next_start = p + 1;           /* next search starts after this prime */

        uint64_t m = p - 1;
        uint64_t g = largest_prime_factor(m);          /* greatest prime factor of m */
        uint64_t t = m / g;           /* (p-1)/g */

        /* must have at least one 2 and one 5 */
        if ((t % 10) != 0) continue;

        /* remove all 2's and 5's */
        while ((t % 2) == 0) t /= 2;
        while ((t % 5) == 0) t /= 5;

        /* remaining part must be 1 */
        if (t != 1) continue;

        /* store the term */
        if (len == cap) {
            size_t new_cap = (cap == 0) ? 8 : cap * 2;
            grow_seq(&terms, &cap, new_cap);
        }
        terms[len++] = (seq_t)p;
    }

    return terms[n - 1];
}
