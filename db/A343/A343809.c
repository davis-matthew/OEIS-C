/*
 * A343809  Divide the positive integers into subsets of lengths given by
 *          successive primes, then reverse the order of terms in each subset.
 * https://oeis.org/A343809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 2*S_{k-1} + p_k - n + 1, where S_k = Σ_{i=1}^{k} p_i (p_i
 *            is the i‑th prime). The block number k is the smallest index
 *            with S_k ≥ n. This implements the construction “divide the
 *            positive integers into blocks of successive prime lengths and
 *            reverse each block”
 * Range:     u8 n <= 238, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* ----------------------------------------------------------------------- */
/*  Prime generation with cumulative sums                                    */
/* ----------------------------------------------------------------------- */
static uint32_t  *prime_list   = NULL;   /* stored primes                 */
static uint64_t  *cum_sums     = NULL;   /* prefix sums of primes         */
static size_t    prime_count   = 0;       /* number of primes generated    */
static size_t    prime_cap     = 0;       /* allocated capacity            */

/* Ensure the arrays hold at least 'needed' entries.  If needed > current
   capacity, double the capacity until it is large enough.                     */
static void
ensure_prime_capacity(size_t needed)
{
    if (prime_cap >= needed)
        return;

    size_t new_cap = prime_cap ? prime_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;

    uint32_t *new_pl = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
    if (!new_pl)
        abort();                          /* out of memory */

    uint64_t *new_cs = (uint64_t *)realloc(cum_sums, new_cap * sizeof(uint64_t));
    if (!new_cs)
        abort();

    prime_list   = new_pl;
    cum_sums     = new_cs;
    prime_cap    = new_cap;
}

/* Test whether 'candidate' is prime, using the primes already found.
   Only trial division up to sqrt(candidate) is performed.                     */
static uint32_t
next_prime_candidate(uint32_t candidate)
{
    for (;; ++candidate) {
        uint64_t sq = (uint64_t)candidate * candidate;
        for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * prime_list[i] <= sq; ++i) {
            if (candidate % prime_list[i] == 0)
                goto next;
        }
        return candidate;           /* prime */
    next:
        continue;
    }
}

/* Extend the prime list (and cumulative sums) until the accumulated sum
   of primes reaches at least 'target'.                                     */
static void
extend_primes_to(uint64_t target)
{
    if (prime_count == 0) {
        /* First prime is 2 */
        uint32_t p = 2;
        ensure_prime_capacity(1);
        prime_list[0] = p;
        cum_sums[0]   = p;
        prime_count   = 1;
    }

    while (cum_sums[prime_count - 1] < target) {
        uint32_t last = prime_list[prime_count - 1];
        uint32_t p = next_prime_candidate(last + 1);

        ensure_prime_capacity(prime_count + 1);
        prime_list[prime_count] = p;
        cum_sums[prime_count]   = cum_sums[prime_count - 1] + p;
        ++prime_count;
    }
}

/* ----------------------------------------------------------------------- */
/*  Main sequence function                                                   */
/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)            /* undefined behaviour for n=0, handled for safety */
        return (seq_t)0;

    extend_primes_to((uint64_t)n);

    /* Find the block that contains the index n.  The cumulative sums are
       stored in increasing order, so a linear search is fast enough for
       the handful of primes needed (≤~200 for n≤10 000). */
    size_t block = 0;
    while (cum_sums[block] < (uint64_t)n)
        ++block;   /* block is zero‑based index of the p_k prime */

    uint64_t S_prev = (block == 0) ? 0ULL : cum_sums[block - 1];
    uint32_t p      = prime_list[block];

    /* Compute the term in the seq_t domain.  All arithmetic is performed
       in seq_t to respect the build’s integer type. */
    seq_t sn   = (seq_t)n;
    seq_t S    = (seq_t)S_prev;
    seq_t pp   = (seq_t)p;
    seq_t res  = 2 * S;
    res += pp;
    res -= sn;
    res += 1;

    return res;
}
