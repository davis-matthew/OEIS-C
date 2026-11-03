/*
 * A337080  Complement of A337037.
 * https://oeis.org/A337080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-03)
 * Helpers:   ipow_i32, is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function provides the terms of OEIS A337080.
   A(n) is the nth composite integer that admits at least two
   unordered factorizations whose sums of factors are equal.
   The implementation scans the integers sequentially, skipping
   primes, and checks each composite using a recursive enumeration
   of all unordered factorizations.  The results are memoised so
   that later calls need only continue the search. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Recursive helper that enumerates all unordered factorizations of
   rem.  For each complete factorisation, cur_sum is the sum of the
   factors.  If a duplicate sum is found, *found is set to true and
   the recursion unwinds immediately.  The array sums collects all
   encountered sums up to a small limit; this is sufficient for the
   modest sizes of numbers that appear in the sequence up to index
   10 000. */
static bool has_duplicate_sum_recur(seq_t rem, seq_t cur_sum, seq_t min_f,
                                    seq_t *sums, size_t *len, bool *found)
{
    if (*found) return true;

    if (rem == 1) {
        /* A complete factorisation: check against previous sums. */
        for (size_t i = 0; i < *len; ++i)
            if (sums[i] == cur_sum) { *found = true; return true; }
        if (*len < 512) sums[(*len)++] = cur_sum;
        return false;
    }

    /* Option: use rem itself as the final factor (always allowed). */
    if (rem >= min_f)
        if (has_duplicate_sum_recur(1, cur_sum + rem, rem,
                                    sums, len, found))
            return true;

    /* Try proper divisors of rem, preserving non‑decreasing order. */
    seq_t sq = floor_sqrt(rem);
    for (seq_t f = min_f; f <= sq; ++f)
        if (rem % f == 0)
            if (has_duplicate_sum_recur(rem / f, cur_sum + f, f,
                                        sums, len, found))
                return true;

    return false;
}

/* Returns true if n is a composite that has two unordered
   factorizations with equal sums. */
static bool has_duplicate_sum(seq_t n)
{
    seq_t sums[512];
    size_t len = 0;
    bool found = false;
    /* Start with an empty sum and the smallest possible factor 2. */
    has_duplicate_sum_recur(n, 0, (seq_t)2, sums, &len, &found);
    return found;
}

/* --------------------------------------------------------------------- */
/*  Main sequence function                                                */
seq_t a(idx_t n)
{
    /* Memoised list of found terms. */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static idx_t last_tested = 3;   /* 3 is the last integer examined */

    /* Extend the list until we have at least n terms. */
    while ((idx_t)seq_len < n) {
        ++last_tested;
        if (is_prime(last_tested)) continue;   /* skip primes */

        if (has_duplicate_sum((seq_t)last_tested)) {
            /* Ensure capacity. */
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 64;
                /* Cast required to satisfy C++ compilation. */
                seq = mem_resize(seq_t, seq, seq_cap);
            }
            seq[seq_len++] = (seq_t)last_tested;
        }
    }
    return seq[n - 1];
}
