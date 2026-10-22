/*
 * A345727  a(n) = (prime(n)+1) * prime(n+1).
 * https://oeis.org/A345727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)+1) * prime(n+1)
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Pre‑allocate space for enough primes (the 10000th prime ≈ 104 729). */
#define MAX_PRIMES 20000

/* Table of primes found so far, in increasing order. */
static uint64_t primes[MAX_PRIMES];
static size_t   prime_cnt = 0;      /* number of primes stored */

/* Next odd candidate to test for primality.  After the first prime (2) is
   inserted, this points to the next odd number to try. */
static uint64_t next_candidate = 3;

/* Test whether x is prime using the primes already discovered. */
static bool is_prime(uint64_t x)
{
    if (x < 2) return false;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Initialise the table with the first prime (2) if it hasn't already been
   set up.  The very first call to a() will trigger this. */
static void init_primes(void)
{
    if (prime_cnt == 0) {
        primes[prime_cnt++] = 2;
        next_candidate = 3;
    }
}

/* Ensure that primes[0 … needed] are available.  Needed is a 0‑based
   index, so for a(n) we request prime indices up to n. */
static void ensure_primes(idx_t needed)
{
    init_primes();

    while (prime_cnt <= (size_t)needed) {
        /* Skip non‑primes, advancing by odd steps to stay on odd numbers. */
        while (!is_prime(next_candidate)) {
            next_candidate += 2;
        }

        if (prime_cnt < MAX_PRIMES) {
            primes[prime_cnt++] = next_candidate;
            next_candidate += 2;   /* advance to the next odd candidate */
        } else {
            /* Capacity reached; further primes are unnecessary for the tests. */
            break;
        }
    }
}

/* OEIS entry A345727: a(n) = (prime(n)+1) * prime(n+1)   (n ≥ 1). */
seq_t a(idx_t n)
{
    /* prime(n) is stored at index n‑1, prime(n+1) at index n. */
    ensure_primes(n);

    seq_t pn  = (seq_t)primes[(size_t)(n - 1)];
    seq_t pn1 = (seq_t)primes[(size_t)n];

    return (pn + 1) * pn1;
}
