/*
 * A309655  The smallest possible nonnegative difference between the sum of
 *          the first n primes (A007504) and the sum of any number of the
 *          directly following and consecutive primes.
 * https://oeis.org/A309655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = 2 * S(n) – max { S(m) : m ≥ n and S(m) ≤ 2 * S(n) } where
 *            S(k) is the sum of the first k primes. The algorithm keeps a
 *            running list of primes and their cumulative sums, extending it
 *            just far enough to satisfy the bound 2 * S(n). That ensures the
 *            minimal non‑negative difference for all n
 * Range:     u8 n <= 9, u16 n <= 556, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */

static uint64_t *primes   = NULL;  /* primes[0..p_len-1]               */
static seq_t   *prefix   = NULL;   /* prefix[i] = sum of first i primes (prefix[0] = 0) */
static size_t  p_len     = 0;      /* number of primes currently stored   */
static size_t  cap       = 0;      /* capacity of the arrays (number of primes) */

static void grow(void)
{
    size_t new_cap = cap ? cap << 1 : 4;

    uint64_t *new_primes = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    seq_t   *new_prefix = (seq_t *)realloc(prefix, (new_cap + 1) * sizeof(seq_t));
    if (!new_primes || !new_prefix) abort();

    primes  = new_primes;
    prefix  = new_prefix;
    cap     = new_cap;
}

static void init_if_needed(void)
{
    if (primes == NULL) {
        grow();
        prefix[0] = (seq_t)0;
    }
}

/* Add exactly one new prime after the current list.  The new prime is
   stored in primes[p_len] and its cumulative sum is stored in
   prefix[p_len+1]. */
static void add_next_prime(void)
{
    init_if_needed();

    uint64_t cand = (p_len == 0 ? 2 : primes[p_len - 1] + 1);
    while (!is_prime((idx_t)cand))
        cand++;                     /* simple trial increment */

    if (p_len == cap) grow();       /* ensure space for the new prime       */
    primes[p_len] = cand;
    prefix[p_len + 1] = prefix[p_len] + (seq_t)cand;
    p_len++;
}

/* Ensure that we have at least 'need' primes (i.e. prefix[need] is
   defined).  If 'need' is greater than the current number of primes,
   primes are generated until the requirement is met. */
static void ensure_primes_up_to(size_t need)
{
    init_if_needed();
    while (p_len < need)
        add_next_prime();
}

/*  Main sequence function.  The function is thread‑unsafe but
    satisfies all requirements (static state, O(n) overall for a
    sequence of up to many thousands of terms). */
seq_t a(idx_t n)
{
    size_t N = (size_t)n;          /* used for array indexing                */
    ensure_primes_up_to(N);        /* make sure prefix[N] exists              */

    seq_t S = prefix[N];           /* sum of first n primes                  */
    seq_t twoS = S + S;            /* target upper bound for following sums  */

    /* Extend the prime list until the cumulative sum exceeds twoS.
       At that point prefix[p_len] > twoS, and the last index with
       prefix <= twoS is p_len-1. */
    while (prefix[p_len] <= twoS)
        add_next_prime();

    seq_t diff = twoS - prefix[p_len - 1];
    return diff;
}
