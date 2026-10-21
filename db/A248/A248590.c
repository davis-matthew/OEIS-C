/*
 * A248590  Least positive integer m such that prime(m) == 1 (mod m + n).
 * https://oeis.org/A248590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the minimal positive integer m such that * prime(m) ≡ 1
 *            (mod m + n), where prime(m) is the m‑th prime
 * Range:     u8 n <= 28, u16 n <= 1240, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Dynamic storage for the first m primes (1‑based). */
static uint64_t *prime_vals = NULL;    /* array of prime values */
static size_t prime_cap   = 0;         /* allocated capacity */
static size_t prime_cnt   = 0;         /* number of primes stored */

/* Ensure prime_vals has at least new_cap elements. */
static void grow_prime_array(size_t new_cap)
{
    if (new_cap <= prime_cap)
        return;
    size_t cap = (prime_cap == 0) ? 16 : prime_cap * 2;
    while (cap < new_cap)
        cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(prime_vals, cap * sizeof(uint64_t));
    if (!tmp)
        exit(1);            /* allocation failure */
    prime_vals = tmp;
    prime_cap  = cap;
}

/* Append next prime to the array. */
static void append_prime(uint64_t p)
{
    if (prime_cnt == prime_cap)
        grow_prime_array(prime_cap + 1);
    prime_vals[prime_cnt++] = p;
}

/* Sieve the next primes until we have at least 'need' primes. */
static void ensure_primes(size_t need)
{
    if (prime_cnt >= need)
        return;

    uint64_t candidate;
    if (prime_cnt == 0)
        candidate = 2;
    else
        candidate = prime_vals[prime_cnt - 1] + 1;

    while (prime_cnt < need)
    {
        if (candidate < 2)
            candidate = 2;
        if (candidate == 2) {
            append_prime(2);
            ++candidate;
            continue;
        }
        if ((candidate & 1) == 0) {           /* skip even numbers */
            ++candidate;
            continue;
        }

        bool is_p = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_vals[i];
            if (p * p > candidate)
                break;
            if (candidate % p == 0) {
                is_p = false;
                break;
            }
        }
        if (is_p)
            append_prime(candidate);
        ++candidate;
    }
}

/* Main OEIS sequence function. */
seq_t a(idx_t n)
{
    /* n is 1‑based as per OEIS offset. */
    for (uint64_t m = 1; ; ++m) {
        /* Make sure the m‑th prime is available. */
        ensure_primes((size_t)m);
        uint64_t p = prime_vals[m - 1];          /* 1‑based index */
        if ((p % (m + (uint64_t)n)) == 1ULL)
            return (seq_t)m;                    /* cast to seq_t */
    }
}
