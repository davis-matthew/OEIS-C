/*
 * A265350  Numbers whose factorial base representation (A007623) contains at
 *          least one of the nonzero digits occurs more than once (although
 *          not necessarily in adjacent positions).
 * https://oeis.org/A265350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 10080, u32 n <= 10080, u64 n <= 10080,
 *            u128 n <= 10080, big any n
 * Validated: standard, pass, 93 terms through n = 9817 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of OEIS A265350 by enumerating natural numbers
   and testing whether the factorial base representation contains any
   non‑zero digit twice.  The list is generated lazily and cached so that
   each integer is examined at most once, regardless of call order.  */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: ensure that the dynamic array has room for *needed* entries. */
static void ensure_cap_seq(size_t needed, seq_t **arr, size_t *cap)
{
    if (needed > *cap) {
        size_t new_cap = *cap ? (*cap * 2) : 16;
        if (new_cap < needed)
            new_cap = needed;
        seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Helper: test if val has a repeated non‑zero digit in its factorial base. */
static bool has_repeated_nonzero_digit(seq_t val)
{
    /* Precompute factorials up to the largest needed for the current valuation. */
    static seq_t facts[21];
    static int maxfact = 1;          /* facts[1] = 1! = 1 */
    static bool initialized = false;

    if (!initialized) {
        facts[1] = (seq_t)1;
        initialized = true;
    }

    /* Extend factorial table if necessary. */
    while (maxfact < 20 && facts[maxfact] <= val) {
        ++maxfact;
        facts[maxfact] = facts[maxfact - 1] * (seq_t)maxfact;
    }

    /* Count non‑zero digits; array index corresponds to digit value. */
    unsigned int digit_seen[21] = {0};

    for (int k = maxfact; k >= 1; --k) {
        seq_t f   = facts[k];
        seq_t dig = val / f;
        if (dig != 0) {
            unsigned int d = (unsigned int)dig;
            if (digit_seen[d])
                return true;          /* repeated digit found */
            digit_seen[d] = 1;
        }
        val %= f;
    }
    return false;
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    /* Static state to cache generated sequence values. */
    static seq_t *seq_arr = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;
    static idx_t candidate = 2;      /* smallest number to test (1 and 2 already skipped) */

    if (n == 0) /* undefined, but guard anyway */
        return (seq_t)0;

    /* Generate terms until we reach the requested index. */
    while (seq_len < (size_t)n) {
        if (has_repeated_nonzero_digit((seq_t)candidate)) {
            ensure_cap_seq(seq_len + 1, &seq_arr, &seq_cap);
            seq_arr[seq_len++] = (seq_t)candidate;
        }
        ++candidate;
    }
    return seq_arr[n - 1];
}
