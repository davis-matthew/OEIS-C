/*
 * A340386  Heinz numbers of integer partitions with an odd number of parts,
 *          the greatest of which is odd.
 * https://oeis.org/A340386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined as the list of positive integers
 * whose prime factorisation corresponds to a partition with an
 * odd number of parts and an odd maximum part.  That is:
 * for a number m = Π prime(p_i)^(e_i),
 *   (Σ e_i) mod 2 = 1   &&   (max_i p_i) mod 2 = 1.
 * The sequence is obtained by taking the first such integer,
 * the second, the third, and so on.  For n ≥ 1 the value a(n)
 * is the n‑th element of this list (offset 1). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* Static state for caching computed terms */
static seq_t *g_cache     = NULL;    /* array of terms */
static size_t g_cache_size = 0;      /* number of stored terms */
static size_t g_cache_cap  = 0;      /* allocated capacity */

/* Pointer to pre‑computed list of primes and its length */
static uint64_t *g_primes    = NULL;
static size_t    g_prime_cnt = 0;

/* The next integer to test for membership in the sequence */
static idx_t g_next_candidate = 2;

/* initialise the list of primes once */
static void init_primes(void)
{
    if (!g_primes) {
        /* a generous upper bound; sufficient for the first 10 000 terms */
        build_primes_upto((idx_t)2000000, &g_primes, &g_prime_cnt);
    }
}

/* append a new term to the cache, growing the array if necessary */
static void cache_append(seq_t val)
{
    if (g_cache_size == g_cache_cap) {
        size_t new_cap = g_cache_cap ? g_cache_cap * 2 : 128;
        seq_t *new_arr = (seq_t *)realloc(g_cache, new_cap * sizeof(seq_t));
        if (!new_arr) exit(1);
        g_cache = new_arr;
        g_cache_cap = new_cap;
    }
    g_cache[g_cache_size++] = val;
}

/* true iff n satisfies the parity conditions */
static bool is_valid(idx_t n)
{
    idx_t tmp = n;
    int parity = 0;           /* sum of exponents mod 2 */
    idx_t max_index = 0;       /* largest prime index with a non‑zero exponent */

    /* trial division by the known primes */
    for (size_t i = 0; i < g_prime_cnt && g_primes[i] * g_primes[i] <= tmp; ++i) {
        if (tmp % g_primes[i] == 0) {
            idx_t exp = 0;
            while (tmp % g_primes[i] == 0) {
                tmp /= g_primes[i];
                ++exp;
            }
            parity ^= (int)(exp & 1);
            idx_t idx = (idx_t)(i + 1);        /* primes are 1‑based indexed */
            if (idx > max_index) max_index = idx;
        }
    }
    /* any remaining prime factor > sqrt(original n) */
    if (tmp > 1) {
        /* find its 1‑based index in the prime list */
        size_t lo = 0, hi = g_prime_cnt;
        while (lo < hi) {
            size_t mid = (lo + hi) / 2;
            if (g_primes[mid] < tmp) lo = mid + 1;
            else hi = mid;
        }
        idx_t idx = (idx_t)(lo + 1);              /* lo points past found prime */
        /* if tmp is larger than any pre‑computed prime, lo==g_prime_cnt */
        if (idx > g_prime_cnt) idx = g_prime_cnt + 1;   /* a safe dummy index */
        parity ^= 1;           /* exponent is 1 */
        if (idx > max_index) max_index = idx;
    }

    return (parity == 1) && ((max_index & 1) == 1);
}

/* ------------------------------------------------------------ */
/*  External function required by the framework                 */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_primes();

    /* Populate the cache until we have at least n terms */
    while (g_cache_size < (size_t)n) {
        /* find the next integer that is valid */
        while (true) {
            if (is_valid(g_next_candidate)) {
                cache_append((seq_t)g_next_candidate);
                break;
            }
            ++g_next_candidate;
        }
        ++g_next_candidate;   /* advance past the one that was just used */
    }

    return g_cache[n - 1];
}
