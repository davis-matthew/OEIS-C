/*
 * A327092  Numbers k that are neither prime nor semiprime, and one of n-1 and
 *          n+1 is prime while the other is semiprime.
 * https://oeis.org/A327092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer k that satisfies: (k is not prime and
 *            not semiprime) and ((k-1 is prime AND k+1 is semiprime) OR (k-1
 *            is semiprime AND k+1 is prime))
 * Range:     u8 n <= 37, u16 n <= 4046, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------ */
/* Simple primality test suitable for the small numbers that
   occur in the first few tens of thousands of elements. */
static bool is_prime_simple(idx_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return (n == 2);
    for (idx_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return false;
    return true;
}

/* Test whether n is semiprime (product of exactly two primes,
   counting multiplicity). */
static bool is_semiprime(idx_t n)
{
    if (n < 4) return false;            /* 4 = 2*2 is the smallest semiprime */
    for (idx_t i = 2; i <= n / i; ++i)
    {
        if (is_prime_simple(i) && n % i == 0)
        {
            idx_t q = n / i;
            return is_prime_simple(q);
        }
    }
    return false;
}

/* ------------------------------------------------------------ */
/* Cached sequence values and generation state. */
static seq_t *seq_values = NULL;   /* array of computed terms   */
static size_t seq_len = 0;         /* how many terms stored    */
static size_t seq_cap = 0;         /* current capacity of array*/
static idx_t next_candidate = 2;   /* next integer to test for membership */

/* Ensure that seq_values holds at least target terms. */
static void ensure_terms(idx_t target)
{
    if (seq_len >= (size_t)target) return;

    /* Grow the array if necessary. */
    if (seq_cap == 0) {
        seq_cap = 64;
        seq_values = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq_values) exit(EXIT_FAILURE);
    }
    while (seq_cap < (size_t)target) {
        seq_cap *= 2;
        seq_values = (seq_t *)realloc(seq_values, seq_cap * sizeof(seq_t));
        if (!seq_values) exit(EXIT_FAILURE);
    }

    /* Generate terms until we have enough. */
    while (seq_len < (size_t)target) {
        idx_t n = next_candidate;

        /* Check the definition. */
        if (!is_prime_simple(n) && !is_semiprime(n)) {
            bool left_prime  = is_prime_simple(n - 1);
            bool right_prime = is_prime_simple(n + 1);
            bool left_semiprime  = is_semiprime(n - 1);
            bool right_semiprime = is_semiprime(n + 1);

            if ((left_prime  && right_semiprime) ||
                (left_semiprime && right_prime)) {
                seq_values[seq_len++] = (seq_t)n;
            }
        }
        ++next_candidate;
    }
}

/* ------------------------------------------------------------ */
/* Public entry point demanded by the specification. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n = 0, safe guard */
    ensure_terms(n);
    return seq_values[(size_t)(n - 1)];
}
