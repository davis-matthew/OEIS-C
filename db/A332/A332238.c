/*
 * A332238  a(n) = n^(n-1) - Sum_{k=1..n-1} k^(k-1) * a(n-k).
 * https://oeis.org/A332238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A332238
   Formula: a(n) = n^(n-1) - Σ_{k=1}^{n-1} k^(k-1) * a(n-k)
   Initial values: a(1)=1, a(2)=1, a(3)=6, a(4)=47, a(5)=493, … */

/* This implementation memoises results so that calls may appear in any order.
   It uses only the allowed integer operations and the utility function
   pow_seq from ../utils/pow_seq.h to compute powers. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow_seq.h"

static seq_t *cache      = NULL;   /* computed a(n) values, indexed from 1 */
static seq_t *pow_cache  = NULL;   /* k^(k-1) for k ≥ 1, indexed from 0 (unused) */
static size_t cap        = 0;      /* size of allocated arrays */
static idx_t  max_comp   = 0;      /* highest n whose a(n) is known */

/* Ensure that cache[1..n] and pow_cache[1..n] are allocated. */
static void ensure_capacity(idx_t n)
{
    /* allocate enough slots (array indices 0..n) */
    if ((size_t)n + 1 <= cap)
        return;

    size_t new_cap = (cap == 0) ? 16 : cap << 1;
    while (new_cap <= (size_t)n + 1)
        new_cap <<= 1;

    /* realloc may return void*; cast it to seq_t* to satisfy C++ */
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    seq_t *new_pow   = (seq_t*)realloc(pow_cache, new_cap * sizeof(seq_t));

    /* if allocation fails, keep the old tables (calls will fail later) */
    if (!new_cache && !new_pow) {
        return;
    }
    cache     = new_cache;
    pow_cache = new_pow;
    cap       = new_cap;

    /* initialise the new entries */
    for (size_t i = 0; i < new_cap; ++i) {
        if (i == 0) {
            pow_cache[i] = 0;          /* unused slot 0 */
        } else {
            pow_cache[i] = pow_seq((seq_t)i, (int)(i - 1));
        }
    }
}

/* Return the nth term of the sequence (n ≥ 1).  The function is
   safe for calls in any order because it memoises preceding values. */
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for the offset, harmless to return 0 */
        return (seq_t)0;

    ensure_capacity(n);

    /* compute missing values up to n if necessary */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (size_t k = 1; k < (size_t)i; ++k) {
            sum += pow_cache[k] * cache[(size_t)i - k];
        }
        cache[(size_t)i] = pow_cache[(size_t)i] - sum;
    }
    max_comp = n;
    return cache[(size_t)n];
}
