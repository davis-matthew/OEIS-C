/*
 * A259624  Strictly increasing list of F - 1, F, and F + 1, where F =
 *          A000045, the Fibonacci numbers.
 * https://oeis.org/A259624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 64, u32 n <= 133, u64 n <= 271,
 *            u128 n <= 550, big any n
 * Validated: standard, pass, 71 terms through n = 1894 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A259624: Strictly increasing list of F-1, F, F+1 where F = Fibonacci numbers
   (F(0)=0, F(1)=1).  The function returns the n‑th term (1‑based). */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

/* Internal helper that inserts a value into a sorted unique array. */
static void insert_unique(seq_t val, seq_t **arr, size_t *len, size_t *cap)
{
    /* Binary search for insertion point. */
    size_t left = 0, right = *len;
    while (left < right) {
        size_t mid = (left + right) >> 1;
        if ((*arr)[mid] < val)
            left = mid + 1;
        else
            right = mid;
    }
    /* If the value already exists, do nothing. */
    if (left < *len && (*arr)[left] == val)
        return;

    /* Ensure capacity. */
    if (*len + 1 > *cap) {
        *cap = (*cap ? *cap * 2 : 4);
        *arr = mem_resize(seq_t, *arr, (*cap * sizeof(seq_t)) / sizeof(seq_t));
    }

    /* Shift elements to the right, if needed, and insert the value. */
    if (*len > left)
        mem_move(&(*arr)[left + 1], &(*arr)[left], (*len - left) * sizeof(seq_t));
    (*arr)[left] = val;
    (*len)++;
}

/* Computes the n-th term of A259624 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n < 1; safe fallback */
        return (seq_t)0;

    /* Static state: sorted unique values and Fibonacci generation. */
    static seq_t *uniq   = NULL;
    static size_t uniq_len   = 0;
    static size_t uniq_cap   = 0;
    static seq_t fib_prev, fib_curr;
    static bool  initialized = false;

    if (!initialized) {
        /* Allocate initial buffer. */
        uniq_cap = 8;
        uniq = mem_resize(seq_t, uniq, uniq_cap);

        /* First few Fibonacci numbers (0,1,1).  Add their F‑1, F, F+1 values. */
        insert_unique((seq_t)0, &uniq, &uniq_len, &uniq_cap);            /* 0   */
        insert_unique((seq_t)1, &uniq, &uniq_len, &uniq_cap);            /* 1   */
        insert_unique((seq_t)2, &uniq, &uniq_len, &uniq_cap);            /* 2   */

        /* Start generating from F(1)=1, F(2)=1. */
        fib_prev = (seq_t)1;
        fib_curr = (seq_t)1;

        initialized = true;
    }

    /* Generate Fibonacci numbers until we have enough distinct terms. */
    while (uniq_len < (size_t)n) {
        seq_t next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = next;

        /* For next ≥ 1, insert next‑1, next, next+1.  next may be 1, making next‑1 zero. */
        if (next > (seq_t)0)
            insert_unique(next - (seq_t)1, &uniq, &uniq_len, &uniq_cap);
        insert_unique(next, &uniq, &uniq_len, &uniq_cap);
        insert_unique(next + (seq_t)1, &uniq, &uniq_len, &uniq_cap);
    }

    return uniq[n - 1];
}
