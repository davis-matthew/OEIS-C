/*
 * A356412  First differences of A007770 (happy numbers).
 * https://oeis.org/A356412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9977, u16 n <= 9977, u32 n <= 9977, u64 n <= 9977,
 *            u128 n <= 9977, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) = A007770(n+1) - A007770(n) by generating the n‑th happy number.
   The algorithm tests natural numbers for happiness using the standard
   cycle detection (4 is the customary unhappy sentinel).  The sequence of
   happy numbers is memoised in a static array so that calls in any order
   are fast.  The implementation works for any seq_t and idx_t provided
   by the build environment. */
#include <stdlib.h>
#include <stdbool.h>

/* ----- static storage for memoised happy numbers ----- */
static seq_t *happy_cache = NULL;   /* array of happy numbers */
static size_t happy_cnt  = 0;       /* how many are stored */
static size_t happy_cap  = 0;       /* allocated capacity */
static idx_t  next_check = 1;       /* next integer to test for happiness */

/* ----- utility: grow cache to at least min_cap ----- */
static void ensure_cache_cap(size_t min_cap)
{
    if (happy_cap >= min_cap)
        return;
    size_t new_cap = happy_cap ? happy_cap * 2 : 16;
    if (new_cap < min_cap)
        new_cap = min_cap;
    seq_t *tmp = (seq_t *)realloc(happy_cache, new_cap * sizeof(seq_t));
    happy_cache = tmp;
    happy_cap   = new_cap;
}

/* ----- test whether an unsigned 64‑bit integer is happy ----- */
static bool is_happy_uint64(uint64_t x)
{
    while (x != 1ULL && x != 4ULL) {
        uint64_t sum = 0;
        while (x > 0) {
            uint64_t d = x % 10ULL;
            sum += d * d;
            x /= 10ULL;
        }
        x = sum;
    }
    return x == 1ULL;
}

/* ----- generate happy numbers until we have at least n+1 entries ----- */
static void ensure_n_happy(idx_t n)
{
    while (happy_cnt < (size_t)(n + 1)) {
        /* find next happy number */
        uint64_t cand = (uint64_t)next_check;
        while (!is_happy_uint64(cand))
            ++cand;
        /* store it */
        ensure_cache_cap(happy_cnt + 1);
        happy_cache[happy_cnt++] = (seq_t)cand;
        /* advance the test start to the next integer */
        next_check = cand + 1;
    }
}

/* ----- main function required by the build system ----- */
seq_t a(idx_t n)
{
    /* ensure we know n-th and (n+1)-st happy numbers */
    ensure_n_happy(n);
    ensure_n_happy(n + 1);

    seq_t h_prev = happy_cache[(size_t)(n - 1)];
    seq_t h_curr = happy_cache[(size_t)n];
    return h_curr - h_prev;
}
