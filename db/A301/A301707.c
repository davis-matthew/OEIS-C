/*
 * A301707  a(n) = n * Sum_{k prime<=n} k.
 * https://oeis.org/A301707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 82, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A301707
 * Formula: a(n) = n * Sum_{p prime ≤ n} p
 * a(1) = 0 is the first term (offset 1)
 *
 * This implementation recomputes a simple sieve of Eratosthenes up to the
 * requested index each time the maximum needed index increases.  It remains
 * fully correct for any `seq_t` type and keeps intermediate values within
 * their natural bounds.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ---------- static cache for the sieve ---------- */
static bool *is_prime_cache = NULL;  /* boolean array: 1 if prime */
static idx_t sieve_max = 0;          /* maximum index currently sieved */

/* ---------- helper to (re)build the sieve ---------- */
static void rebuild_sieve(idx_t limit)
{
    /* free any previously allocated array */
    if (is_prime_cache != NULL) {
        free(is_prime_cache);
        is_prime_cache = NULL;
    }

    /* allocate new array (size limit+1 to include index 'limit') */
    size_t size = (size_t)(limit + 1);
    is_prime_cache = (bool *)malloc(size * sizeof(bool));
    if (is_prime_cache == NULL) {
        /* allocation failure – abort computation (undefined for this build) */
        return;
    }

    /* initialise all entries to true, then mark 0 and 1 as non‑prime */
    memset(is_prime_cache, true, size);
    if (limit >= 0) is_prime_cache[0] = false;
    if (limit >= 1) is_prime_cache[1] = false;

    /* classic Eratosthenes sieve */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime_cache[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                is_prime_cache[j] = false;
            }
        }
    }

    sieve_max = limit;
}

/* ---------- main function computing a(n) ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* out of offset, harmless */

    if (n > sieve_max) rebuild_sieve(n);

    seq_t sum = 0;
    /* sum all primes ≤ n */
    for (idx_t i = 2; i <= n; ++i) {
        if (is_prime_cache[i]) {
            sum += (seq_t)i;
        }
    }

    /* compute the final value a(n) = n * sum */
    return (seq_t)n * sum;
}
