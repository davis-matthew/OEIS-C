/*
 * A267504  Primes p such that p-1 is squarefree and all prime divisors of p-1
 *          other than 11 are also in the sequence.
 * https://oeis.org/A267504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes p such that p-1 is squarefree and all prime divisors of
 *            p-1 * other than 11 are also in the sequence. Computed by
 *            incremental search
 * Range:     u8 n <= 8, u16 n <= 41, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   append_candidate, factor_u64, is_squarefree, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Any order of calls is supported.  Sequence index starts at 1. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/is_squarefree.h"
#include "../utils/append_candidate.h"

/* static storage for the sequence */
static seq_t *seq = NULL;          /* dynamic array of sequence values */
static size_t seq_len = 0;         /* current number of stored terms */
static size_t seq_cap = 0;         /* current capacity of seq */

/* binary search to test whether a prime q is already in the sequence */
static bool in_sequence(uint64_t q)
{
    size_t low = 0;
    size_t high = seq_len;          /* one past the last element */
    while (low < high) {
        size_t mid = low + (high - low) / 2;
        if (seq[mid] < (seq_t)q)
            low = mid + 1;
        else
            high = mid;
    }
    return (low < seq_len && seq[low] == (seq_t)q);
}

/* test whether a prime p is a member of the sequence */
static bool good_prime(uint64_t p)
{
    uint64_t m = p - 1;

    /* p-1 must be squarefree */
    if (!is_squarefree(m))
        return false;

    /* factor p-1 */
    uint64_t primes[128];
    uint64_t exps[128];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = primes[i];
        if (q == 11)          /* 11 is exempt */
            continue;
        if (!in_sequence(q))
            return false;     /* required prime not yet in sequence */
    }
    return true;
}

/* return the nth term of the sequence (n ≥ 1) */
seq_t a(idx_t n)
{
    /* extend the sequence until it contains the n‑th term */
    while (seq_len < (size_t)n) {
        uint64_t start = (seq_len == 0) ? 1ULL : (uint64_t)seq[seq_len - 1];
        uint64_t p = next_prime(start);     /* first prime ≥ start */

        /* find the next prime that satisfies the definition */
        while (!good_prime(p))
            p = next_prime(p);

        /* store the prime in the sequence */
        append_candidate(&seq, &seq_len, &seq_cap, (seq_t)p);
    }

    return seq[n - 1];
}
