/*
 * A350168  Count from 0 to 1st prime 2, then from 0 to 2nd prime 3, then from
 *          0 to 3rd prime 5, etc ...
 * https://oeis.org/A350168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the offset within the block of length p_k+1 * containing
 *            n, where p_k is the k‑th prime. For n=1,2,3 the * block is
 *            0,1,2; for n=4…7 the block is 0,1,2,3; for n=8…13 * the block is
 *            0,1,2,3,4,5; and so on
 * Range:     u8 n <= 6391, u16 n <= 9340, u32 n <= 9340, u64 n <= 9340,
 *            u128 n <= 9340, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *primes = NULL;       /* stores primes p_1, p_2, …         */
static idx_t   *cumulative = NULL;   /* cumulative lengths T(k)           */
static size_t   primes_cnt = 0;       /* number of generated primes       */
static size_t   primes_cap = 0;       /* allocated capacity of arrays     */

/* Ensure enough capacity for primes_cnt+1 entries. */
static void ensure_capacity(size_t needed)
{
    if (primes_cap < needed)
    {
        size_t new_cap = (primes_cap == 0) ? 8 : primes_cap * 2;
        while (new_cap < needed) new_cap <<= 1;

        uint64_t *p = (uint64_t*)realloc(primes, new_cap * sizeof(uint64_t));
        if (!p) exit(EXIT_FAILURE);
        primes = p;

        idx_t *c = (idx_t*)realloc(cumulative, new_cap * sizeof(idx_t));
        if (!c) exit(EXIT_FAILURE);
        cumulative = c;

        primes_cap = new_cap;
    }
}

/* Test whether candidate is prime using known smaller primes. */
static bool is_prime_candidate(uint64_t candidate)
{
    for (size_t i = 0; i < primes_cnt; ++i)
    {
        uint64_t p = primes[i];
        if (p * p > candidate) break;
        if (candidate % p == 0) return false;
    }
    return true;
}

/* Generate the next prime after the last one stored.  Updates both
   primes[] and cumulative[]. */
static void generate_next_prime(void)
{
    uint64_t cand;
    if (primes_cnt == 0)
        cand = 2;
    else
    {
        cand = primes[primes_cnt - 1] + 1;
        if (cand <= 2) cand = 2;
        else if ((cand & 1) == 0) cand += 1;   /* start at an odd number */
    }

    /* Find the next prime. */
    while (!is_prime_candidate(cand))
        cand += 2;   /* skip even numbers */

    ensure_capacity(primes_cnt + 1);

    primes[primes_cnt] = cand;
    idx_t new_cum = (primes_cnt == 0)
        ? ((idx_t)cand + 1)
        : cumulative[primes_cnt - 1] + ((idx_t)cand + 1);
    cumulative[primes_cnt] = new_cum;
    ++primes_cnt;
}

/* Return the n-th term of the sequence.  n is 1‑based. */
seq_t a(idx_t n)
{
    /* Generate primes until the cumulative length covers n. */
    while (primes_cnt == 0 || cumulative[primes_cnt - 1] < n)
        generate_next_prime();

    /* Locate the block containing n. */
    size_t idx = 0;
    while (idx < primes_cnt && cumulative[idx] < n)
        ++idx;

    idx_t prev_cum = (idx == 0) ? (idx_t)0 : cumulative[idx - 1];
    idx_t offset   = n - prev_cum - 1;   /* 0‑based offset within the block */

    return (seq_t)offset;
}
