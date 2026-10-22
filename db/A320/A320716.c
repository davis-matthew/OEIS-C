/*
 * A320716  Indices of primes followed by a gap (distance to next larger
 *          prime) of 36.
 * https://oeis.org/A320716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer i (1‑based) such that the next prime
 *            after the i-th prime is exactly 36 larger
 * Range:     u8 none, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A320716: indices i such that prime(i+1) = prime(i) + 36 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* ---- helpers for dynamic arrays -------------------------------------- */
static void grow_if_needed(void **ptr, size_t *cap, size_t needed, size_t elem_size)
{
    if (*cap >= needed)
        return;
    size_t new_cap = (*cap == 0) ? 16 : *cap * 2;
    while (new_cap < needed)
        new_cap *= 2;
    void *np = realloc(*ptr, new_cap * elem_size);
    if (!np)
        abort();
    *ptr = np;
    *cap = new_cap;
}

/* ---- prime generation ----------------------------------------------- */
static uint64_t *primes          = NULL;   /* growing list of primes   */
static size_t   primes_len       = 0;
static size_t   primes_cap       = 0;
static uint64_t next_candidate   = 2;      /* next number to test      */

static bool is_prime_uint64(uint64_t m)
{
    if (m < 2)
        return false;
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = primes[i];
        if (p * p > m)
            break;
        if (m % p == 0)
            return false;
    }
    return true;
}

/* ---- indices where the next prime is 36 larger ------------------------ */
static uint64_t *gap_indices   = NULL;   /* store indices in 64‑bit form */
static size_t   gap_len        = 0;
static size_t   gap_cap        = 0;

/* Generate at least 'required' members of the sequence. */
static void generate_primes_for_gap(size_t required)
{
    if (gap_len >= required)
        return;

    while (gap_len < required) {
        /* Find the next prime. */
        while (!is_prime_uint64(next_candidate))
            ++next_candidate;

        /* Store the prime. */
        grow_if_needed((void **)&primes, &primes_cap, primes_len + 1, sizeof(uint64_t));
        primes[primes_len++] = next_candidate;

        /* Check if the gap from the previous prime equals 36. */
        if (primes_len >= 2) {
            uint64_t diff = next_candidate - primes[primes_len - 2];
            if (diff == 36) {
                grow_if_needed((void **)&gap_indices, &gap_cap,
                               gap_len + 1, sizeof(uint64_t));
                /* primes_len is 1‑based; the previous prime has index primes_len‑1. */
                gap_indices[gap_len++] = primes_len - 1;
            }
        }
        ++next_candidate;
    }
}

/* ---- public sequence function ---------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                /* unspecified for n < 1 */
        return 0;
    generate_primes_for_gap((size_t)n);
    return (seq_t)gap_indices[n - 1];
}
