/*
 * A352127  a(n) is the smallest even number whose n smallest divisors > 2 are
 *          all odd.
 * https://oeis.org/A352127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * ∏_{j=k+1}^{k+n} prime(j) where k is the smallest
 *            integer * such that 2*prime(k+1) > prime(k+n). The sequence
 *            starts at a(0)=2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Dynamic array of odd primes (p₁=3, p₂=5, …). */
static uint32_t *odd_primes      = NULL;   /* array of primes */
static size_t   odd_cnt          = 0;      /* number of primes stored */
static size_t   odd_cap          = 0;      /* allocated capacity */
static uint32_t odd_next_candidate = 3;    /* next odd number to test */

/* Ensure that at least 'target' odd primes are stored in 'odd_primes'. */
static void ensure_primes_needed(size_t target)
{
    if (odd_cnt >= target)
        return;

    /* Grow the storage buffer as needed. */
    if (odd_cap < target) {
        size_t new_cap = target + 1024;
        /* explicit cast required for C++ compatibility */
        uint32_t *tmp = (uint32_t *)realloc(odd_primes, new_cap * sizeof(uint32_t));
        if (!tmp) abort();   /* out of memory */
        odd_primes = tmp;
        odd_cap    = new_cap;
    }

    /* Continue generating odd primes until we reach 'target'. */
    while (odd_cnt < target) {
        uint32_t candidate = odd_next_candidate;
        odd_next_candidate += 2;   /* skip even numbers */

        /* Check divisibility by already known primes up to sqrt(candidate). */
        bool is_prime = true;
        uint32_t sqrt_candidate = (uint32_t)sqrt((double)candidate);
        for (size_t i = 0; i < odd_cnt; ++i) {
            uint32_t p = odd_primes[i];
            if (p > sqrt_candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            odd_primes[odd_cnt++] = candidate;
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;   /* a(0) = 2 */

    /* For safety: n fits into a size_t for loop bounds. */
    size_t un = (size_t)n;

    size_t k = 0;   /* minimal offset satisfying the inequality */

    /* Find smallest k such that 2 * p_{k+1} > p_{k+n}. */
    for (;;) {
        /* We need primes up to index (k+n), which is 1‑based. */
        size_t required = k + un + 1;   /* index conversion to 0‑based: need odd_primes[required‑1] */
        ensure_primes_needed(required);

        uint32_t p_k1   = odd_primes[k];                  /* p_{k+1} */
        uint32_t p_kn   = odd_primes[k + un - 1];        /* p_{k+n} */

        if ((uint64_t)2 * p_k1 > (uint64_t)p_kn)
            break;
        ++k;
    }

    /* Compute the product: 2 * ∏_{j=k+1}^{k+n} p_j. */
    seq_t prod = (seq_t)2;
    size_t start = k;
    size_t end   = start + un;        /* exclusive */
    for (size_t idx = start; idx < end; ++idx) {
        prod = prod * (seq_t)odd_primes[idx];
    }
    return prod;
}
