/*
 * A335046  Maximal common prime of two Goldbach partitions of 2n and 2(n+1)
 *          or zero (if common prime does not exist).
 * https://oeis.org/A335046
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = maximal common prime of two Goldbach partitions of 2n and 2(n+1),
   returning 0 if no common prime exists.  The routine expands a
   classic Sieve of Eratosthenes lazily and then scans primes in
   descending order.  It works for all n ≥ 2 and is valid C11 and
   C++17 (with seq_t being an unsigned integer or arbitrary‑precision
   integer class). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* static prime sieve: prime_flag[i] is true iff i is prime. */
static bool *prime_flag = NULL;
static size_t prime_cap = 0;

/* Ensure the sieve covers all numbers up to limit (inclusive). */
static void ensure_primes(idx_t limit)
{
    size_t new_cap = (size_t)limit + 1;
    if (new_cap <= prime_cap)
        return;                       /* already sufficient */

    /* Reallocate the flag array (casts work for both C and C++). */
    bool *tmp = (bool *)realloc(prime_flag, new_cap * sizeof(bool));
    if (tmp == NULL) {
        /* Allocation failed – abort to signal error. */
        abort();
    }
    prime_flag = tmp;

    /* Initialize all entries as prime, then mark 0 and 1 as composite. */
    for (size_t i = 0; i < new_cap; ++i)
        prime_flag[i] = true;
    if (new_cap > 0) prime_flag[0] = false;
    if (new_cap > 1) prime_flag[1] = false;

    /* Sieve of Eratosthenes. */
    for (size_t i = 2; i * i < new_cap; ++i) {
        if (!prime_flag[i])
            continue;
        for (size_t j = i * i; j < new_cap; j += i)
            prime_flag[j] = false;
    }

    prime_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;          /* below offset – arbitrary */

    /* We need primes up to 2*(n+1). */
    ensure_primes(2 * (n + 1));

    idx_t e1 = 2 * n;          /* even number 2n   */
    idx_t e2 = e1 + 2;          /* even number 2(n+1) */

    /* Scan primes in descending order. */
    for (idx_t p = e1; p >= 2; --p) {
        if (!prime_flag[p])
            continue;           /* not a prime */

        /* Check if p can appear in a Goldbach partition of e1
           and of e2: i.e. the complementary numbers are prime. */
        if (prime_flag[e1 - p] && prime_flag[e2 - p]) {
            return (seq_t)p;    /* maximal common prime found */
        }
        if (p == 2)               /* avoid unsigned underflow */
            break;
    }

    return (seq_t)0;            /* no common prime */
}
