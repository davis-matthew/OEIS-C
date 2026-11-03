/*
 * A372801  Order of 16 modulo the n-th prime: least k such that prime(n)
 *          divides 16^k-1.
 * https://oeis.org/A372801
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 6711, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9682 (2026-11-01)
 * Helpers:   factorize, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by finding the smallest k such that 16^k ≡ 1 (mod prime(n)) */
/* The order divides prime(n)−1, so we reduce it by the prime factors of that */
/* number.  This method works for any n ≥ 2. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factorize.h"
#include "../utils/powmod.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

seq_t a(idx_t n)
{
    /* offset: for n < 2 the sequence is undefined – return 0 */
    if (n < 2)
        return (seq_t)0;

    /* get the nth prime (prime(1)=2, prime(2)=3, …) */
    uint32_t prime_uint = get_prime(n);
    seq_t p = (seq_t)prime_uint;

    /* initial order is p-1 */
    seq_t order = (seq_t)(p - 1);

    /* factorize p-1; it fits comfortably in 64‑bit */
    uint64_t factors[64];
    int exps[64];
    size_t num_factors = factorize((uint64_t)p - 1, factors, exps);

    /* reduce the order by each prime factor */
    for (size_t i = 0; i < num_factors; ++i) {
        seq_t q = (seq_t)factors[i];
        /* try to divide out powers of q */
        while (order % q == 0) {
            seq_t trial = order / q;
            if (powmod((seq_t)16, (idx_t)trial, p) == (seq_t)1)
                order = trial;          /* division possible – keep reducing */
            else
                break;                  /* cannot divide by more q */
        }
    }

    return order;
}
