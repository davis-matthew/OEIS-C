/*
 * A352872  Numbers whose weakly increasing prime indices y have a fixed point
 *          y(i) = i.
 * https://oeis.org/A352872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A352872 – Numbers whose weakly increasing prime indices
   multiset has a fixed point.  This implementation simply
   iterates over successive integers, tests the fixed‑point
   condition on the multiset of prime indices, and caches the
   results so that repeated calls with arbitrary order of n
   are fast. */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/build_primes.h"

/* Prime table (1‑based indices).  The table is filled once on
   first use.  For n up to ~10000 the numbers we examine stay
   well below two million, which is a safe bound for the prime
   sieve. */
static idx_t  *prime_table = NULL;
static size_t prime_count = 0;
static bool    primes_ready = false;
static const idx_t PRIME_LIMIT = 2000000u;

static void init_primes(void)
{
    if (primes_ready)
        return;
    build_primes(PRIME_LIMIT, &prime_table, &prime_count);
    primes_ready = true;
}

/* Binary search of a prime in prime_table, returns its
   1‑based index.  The function assumes p is actually a prime
   from the table. */
static idx_t prime_index(idx_t p)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_table[mid] >= p)
            hi = mid;
        else
            lo = mid + 1;
    }
    /* lo is the position (0‑based) where prime_table[lo] == p */
    return (idx_t)(lo + 1);
}

/* Returns true if the integer m has a fixed point in its
   prime‑index multiset.  The multiset is sorted by construction
   because we iterate over primes in increasing order. */
static bool has_fixed_prime_index(idx_t m)
{
    /* A small local array is sufficient: even for the
       largest candidate the number of distinct prime indices
       is less than 64 (the 2‑million bound gives at most 148 933
       primes, far below). */
    uint32_t indices[64];
    size_t  idx_len = 0;
    idx_t   tmp = m;

    for (size_t pi = 0; pi < prime_count &&
            (idx_t)prime_table[pi] * (idx_t)prime_table[pi] <= tmp; ++pi) {
        idx_t p = prime_table[pi];
        if (tmp % p == 0) {
            idx_t idx = (idx_t)(pi + 1);   /* 1‑based index of the prime */
            while (tmp % p == 0) {
                tmp /= p;
                if (idx_len < 64)
                    indices[idx_len++] = (uint32_t)idx;
            }
        }
    }
    if (tmp > 1) {            /* remaining factor is a prime > sqrt(original m) */
        idx_t idx = prime_index(tmp);
        if (idx_len < 64)
            indices[idx_len++] = (uint32_t)idx;
    }

    /* The indices array is already sorted ascending.  Check for
       a fixed point: y(i) == i for some i. */
    for (size_t i = 1; i <= idx_len; ++i) {
        if (indices[i-1] == (uint32_t)i)
            return true;
    }
    return false;
}

/* Cache for the sequence.  The cache grows as needed and
   persists across calls. */
static seq_t  *seq_cache = NULL;
static size_t seq_len   = 0;   /* number of terms already stored */
static size_t seq_cap   = 0;   /* capacity of seq_cache */
static idx_t  next_candidate = 2; /* next integer to test */

static void ensure_seq_capacity(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_array = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (seq_cache && seq_len > 0)
        memcpy(new_array, seq_cache, seq_len * sizeof(seq_t));
    free(seq_cache);
    seq_cache = new_array;
    seq_cap   = new_cap;
}

/* ------------------------------------------------------------------ */
/* Main function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    init_primes();

    while (seq_len < (size_t)n) {
        if (has_fixed_prime_index(next_candidate)) {
            ensure_seq_capacity(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return seq_cache[n - 1];
}
