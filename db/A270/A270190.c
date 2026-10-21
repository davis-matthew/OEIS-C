/*
 * A270190  Numbers n for which prime(n+1)-prime(n) is a multiple of three.
 * https://oeis.org/A270190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the index m such that prime(m+1)-prime(m) is a multiple
 *            of 3. The function computes the nth such index by generating
 *            primes sequentially and recording the indices whose
 *            consecutive‑prime difference is divisible by 3
 * Range:     u8 n <= 87, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static data needed for the search */
static uint32_t *primes        = NULL;   /* generated primes */
static size_t   primes_cnt     = 0;      /* number of primes stored */
static size_t   primes_cap     = 0;      /* current capacity of primes array */

static seq_t  *good           = NULL;   /* indices m with prime(m+1)-prime(m) % 3 == 0 */
static size_t   good_cnt      = 0;      /* number of good indices stored */
static size_t   good_cap      = 0;      /* current capacity of good array */

static idx_t  cur_candidate   = 3;      /* next integer to test for primality */

/* Grow the primes array to a larger capacity. */
static void grow_primes(void) {
    size_t new_cap = (primes_cap == 0) ? 64 : primes_cap * 2;
    uint32_t *new_ptr = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
    if (new_ptr) {
        primes = new_ptr;
        primes_cap = new_cap;
    }
}

/* Grow the good array to a larger capacity. */
static void grow_good(void) {
    size_t new_cap = (good_cap == 0) ? 64 : good_cap * 2;
    seq_t *new_ptr = (seq_t *)realloc(good, new_cap * sizeof(seq_t));
    if (new_ptr) {
        good = new_ptr;
        good_cap = new_cap;
    }
}

/* Ensure that the first 'target' good indices have been generated. */
static void ensure_good_cnt(idx_t target) {
    if (good_cnt >= target)
        return;

    /* Initialise data structures on first call */
    if (primes == NULL) {
        primes_cap = 64;
        primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
        if (!primes) return;          /* allocation failure – silently abort */
        primes[0] = 2;
        primes_cnt = 1;

        good_cap = 64;
        good = (seq_t *)malloc(good_cap * sizeof(seq_t));
        if (!good) return;            /* allocation failure – silently abort */

        cur_candidate = 3;
    }

    while (good_cnt < target) {
        /* Find the next prime number */
        while (1) {
            bool composite = false;
            /* Test divisibility by all stored primes up to sqrt(cur_candidate) */
            uint32_t last_prime = primes[primes_cnt - 1];
            uint64_t limit = (uint64_t)last_prime * last_prime;
            for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= cur_candidate; ++i) {
                if (cur_candidate % primes[i] == 0) {
                    composite = true;
                    break;
                }
            }
            if (!composite)
                break;          /* cur_candidate is prime */
            ++cur_candidate;
            if (cur_candidate < 3) cur_candidate = 3;  /* just in case overflow */
        }

        /* Store the new prime */
        if (primes_cnt == primes_cap)
            grow_primes();
        primes[primes_cnt++] = (uint32_t)cur_candidate;

        /* If we have at least two primes, check the diff with the previous one */
        if (primes_cnt >= 2) {
            uint32_t diff = primes[primes_cnt - 1] - primes[primes_cnt - 2];
            if (diff % 3 == 0) {
                seq_t val = (seq_t)(primes_cnt - 1);   /* index of the previous prime */
                if (good_cnt == good_cap)
                    grow_good();
                good[good_cnt++] = val;
            }
        }

        ++cur_candidate;     /* move to next candidate for the next iteration */
    }
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;          /* undefined for n=0, return 0 as safe default */

    ensure_good_cnt(n);
    /* a(n) is stored at good[n-1] because a(1) is the first element */
    return good[n - 1];
}
