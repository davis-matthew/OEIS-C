/*
 * A374950  Sequence starting with 2, where the next term is the lowest new
 *          integer which is greater than 1 and coprime with the sum of all
 *          previous terms.
 * https://oeis.org/A374950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 2;  
   for n > 1, a(n) = min{ m > 1 unused | gcd(m, sum_{i=1}^{n-1} a(i)) = 1 } */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"

/* Static cache for the sequence */
static seq_t *seq = NULL;
static size_t seq_len = 0;      /* number of terms stored */
static size_t seq_cap = 0;

/* Static cache for the used numbers */
static seq_t *used = NULL;
static size_t used_len = 0;     /* number of used numbers stored */
static size_t used_cap = 0;

/* Running sum of stored terms */
static seq_t sum = 0;

/* Ensure seq can hold at least 'needed' terms */
static void ensure_seq_capacity(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 32;
    if (new_cap <= needed)
        new_cap = needed + 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed: terminate */
        exit(EXIT_FAILURE);
    }
    seq = tmp;
    seq_cap = new_cap;
}

/* Ensure used can hold at least 'needed' numbers */
static void ensure_used_capacity(size_t needed)
{
    if (needed <= used_cap)
        return;
    size_t new_cap = used_cap ? used_cap << 1 : 16;
    if (new_cap <= needed)
        new_cap = needed + 1;
    seq_t *tmp = (seq_t *)realloc(used, new_cap * sizeof(seq_t));
    if (!tmp) {
        exit(EXIT_FAILURE);
    }
    used = tmp;
    used_cap = new_cap;
}

/* Check whether a value is already used */
static bool is_used(seq_t val)
{
    for (size_t i = 0; i < used_len; ++i)
        if (used[i] == val)
            return true;
    return false;
}

/* Append a new term to the sequence and mark it as used */
static void append_term(seq_t value)
{
    ensure_seq_capacity(seq_len + 1);
    seq[seq_len++] = value;
    sum += value;

    ensure_used_capacity(used_len + 1);
    used[used_len++] = value;
}

/* Compute the n-th term (1‑based) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined, but avoid UB */

    /* Initialize the cache if empty */
    if (seq_len == 0) {
        ensure_seq_capacity(1);
        ensure_used_capacity(1);
        seq[0] = 2;
        seq_len = 1;
        sum = 2;
        used[0] = 2;
        used_len = 1;
    }

    /* Compute terms up to the requested index */
    while (seq_len < (size_t)n) {
        seq_t cand = 2;
        while (true) {
            if (!is_used(cand) && gcd(cand, sum) == 1)
                break;          /* found the next term */
            ++cand;
        }
        append_term(cand);
    }

    return seq[n - 1];
}
