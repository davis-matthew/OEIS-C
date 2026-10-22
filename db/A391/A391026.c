/*
 * A391026  Even numbers that are neither powerful nor squarefree.
 * https://oeis.org/A391026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 7773, u32 n <= 9999, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A391026: Even numbers that are neither powerful nor squarefree.
 * a(n) = the n-th even integer x such that x is not powerful (has an exponent = 1)
 * and x is not squarefree (has an exponent ≥ 2).  The implementation
 * memoizes computed terms and expands the sequence lazily up to any
 * requested index n.  Prime generation is performed incrementally using
 * trial division on the existing prime list.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Forward declaration of the required sequence function */
seq_t a(idx_t n);

/* ===============================================================
   Static data structures for memoization and prime generation
   =============================================================== */

static seq_t *seq_cache      = NULL;   /* sequence values   */
static size_t seq_len        = 0;      /* number of terms stored */
static size_t seq_cap        = 0;      /* allocated capacity    */

static uint32_t *primes      = NULL;   /* list of primes      */
static size_t    primes_len  = 0;      /* number of primes    */
static idx_t     primes_last = 1;      /* largest candidate 
                                            checked for primality  */

static uint64_t  last_checked = 1;     /* largest integer examined during search */

/* ===============================================================
   Utility: Grow sequence cache
   =============================================================== */
static void grow_seq_cache(void) {
    size_t new_cap = (seq_cap == 0) ? 8 : seq_cap << 1;
    seq_t *new_buf = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_buf) exit(EXIT_FAILURE); /* abort on allocation failure */
    seq_cache = new_buf;
    seq_cap   = new_cap;
}

/* ===============================================================
   Utility: Grow primes list
   =============================================================== */
static void grow_primes(size_t new_len) {
    size_t cap = (primes_len == 0) ? 16 : primes_len << 1;
    while (cap < new_len) cap <<= 1;
    uint32_t *new_buf = (uint32_t *)realloc(primes, cap * sizeof(uint32_t));
    if (!new_buf) exit(EXIT_FAILURE);
    primes = new_buf;
    primes_len = new_len;
}

/* ===============================================================
   Generate primes up to a given limit (inclusive) using trial
   division against already known primes.
   =============================================================== */
static void build_primes_up_to(idx_t limit) {
    if (limit <= primes_last) return;       /* already built */

    /* Ensure we have a workspace for new primes */
    if (primes_len == 0) {
        grow_primes(16);
        /* initialise first primes 2 and 3 */
        primes[0] = 2;
        primes[1] = 3;
        primes_len = 2;
        primes_last = 3;
    }

    for (idx_t cand = primes_last + 1; cand <= limit; ++cand) {
        bool is_prime = true;
        uint64_t sqrt_c = (uint64_t) sqrt((double)cand) + 1;
        for (size_t i = 0; i < primes_len && primes[i] <= sqrt_c; ++i) {
            if (cand % primes[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (primes_len == seq_cap) grow_primes(primes_len + 1);
            primes[primes_len++] = (uint32_t)cand;
        }
    }
    primes_last = limit;
}

/* ===============================================================
   Test whether an even integer x (64‑bit) is neither powerful nor
   squarefree: i.e., x has at least one prime exponent equal to 1
   and at least one prime exponent ≥ 2.
   =============================================================== */
static bool qualifies(uint64_t x) {
    if ((x & 1U) != 0) return false;   /* must be even */

    bool has_one   = false;
    bool has_twoor = false;

    uint64_t temp = x;
    uint64_t limit = (uint64_t) sqrt((double) temp) + 1;
    build_primes_up_to(limit);

    for (size_t i = 0; i < primes_len && (uint64_t)primes[i] <= limit; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                ++exp;
                temp /= p;
            }
            if (exp == 1)      has_one   = true;
            if (exp >= 2)      has_twoor = true;
            if (has_one && has_twoor) return true;
            limit = (uint64_t) sqrt((double) temp) + 1;
        }
    }
    if (temp > 1) { /* remaining prime factor with exponent 1 */
        has_one = true;
    }
    return has_one && has_twoor;
}

/* ===============================================================
   Main function for the sequence.
   =============================================================== */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined but safe */

    /* Grow cache until we have at least n terms */
    while (seq_len < n) {
        ++last_checked;
        uint64_t cand = last_checked;
        if (cand & 1ULL) continue;          /* skip odds */

        if (qualifies(cand)) {
            if (seq_len == seq_cap) grow_seq_cache();
            seq_cache[seq_len++] = (seq_t)cand;
        }
    }
    return seq_cache[n - 1];
}
