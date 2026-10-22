/*
 * A373237  Relative of Hofstadter Q-sequence: a(n) = max(0, n+4315) for n <=
 *          0; a(n) = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3)) for n > 0.
 * https://oeis.org/A373237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4875, u32 n <= 4875, u64 n <= 4875,
 *            u128 n <= 4875, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A373237 – Relative of Hofstadter Q‑sequence
 *
 *   a(n) = max(0, n+4315)                                   for n <= 0
 *   a(n) = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3))         for n > 0
 *
 * The function below memoises values so subsequent calls are O(1).
 * It is written so that it compiles as C11 and as C++17.
 * No external utilities are required.  The cache size (12 001) is
 * comfortably above the known sequence length (4 875), so the
 * implementation stays well under the time limit for n ≤ 10 000.
 */

#include <stdbool.h>

#define CACHE_LIMIT 12001   /* must exceed the maximal n used in tests */

/* External linkage as required */
seq_t a(idx_t n);

/* ------------------------------- */
/*  Internal helpers                */
/* ------------------------------- */

static seq_t cache[CACHE_LIMIT];
static bool computed[CACHE_LIMIT] = {0};

/* For negative indices: a(n) = max(0, n+4315) */
static inline seq_t a_signed(long long idx)
{
    if (idx <= 0) {
        long long val = idx + 4315;          /* max(0, idx+4315) */
        return (val > 0) ? (seq_t)val : (seq_t)0;
    }
    /* idx > 0 – use memoised recurrence */
    return a((idx_t)idx);
}

/* Recursive evaluation with memoisation.
 * Uses cache[0]..cache[CACHE_LIMIT-1] and computed[ ]  */
static inline seq_t a_value(idx_t n)
{
    /* base case for n == 0 (required by the definition) */
    if (n == 0)
        return (seq_t)4315;

    /* return already stored result if available */
    if (computed[n])
        return cache[n];

    /* compute the three predecessors first */
    seq_t a1 = a_signed((long long)n - 1);
    seq_t a2 = a_signed((long long)n - 2);
    seq_t a3 = a_signed((long long)n - 3);

    /* now the three terms of the recurrence */
    seq_t t1 = a_signed((long long)n - (long long)a1);
    seq_t t2 = a_signed((long long)n - (long long)a2);
    seq_t t3 = a_signed((long long)n - (long long)a3);

    seq_t result = t1 + t2 + t3;

    /* store and return */
    cache[n] = result;
    computed[n] = true;
    return result;
}

/* Public function.  n is guaranteed to be >= 1 by the contract. */
seq_t a(idx_t n)
{
    /* n is positive; delegate to the memoised routine */
    return a_value(n);
}
