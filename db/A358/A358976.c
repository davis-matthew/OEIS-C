/*
 * A358976  Numbers that are coprime to the sum of their factorial base digits
 *          (A034968).
 * https://oeis.org/A358976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, sum_fact_base_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A358976 by brute‑force enumeration.
   a(1) = 1, a(2) = 2, ...   */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/gcd.h"
#include "../utils/sum_fact_base_digits.h"

/* Forward declarations */
static seq_t sum_fact_base_digits(seq_t n);
static void push_cache(seq_t val);
static void ensure_terms(idx_t n);

/* Static storage for memoization */
static seq_t *cache   = NULL;   /* array of found terms */
static size_t cache_size = 0;   /* number of terms stored */
static size_t cache_cap  = 0;   /* current allocation capacity */
static seq_t next_test = 1;     /* next integer to test */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* undefined for the sequence, but harmless */
    ensure_terms(n);
    return cache[n - 1];
}

/* ------------------------------------------------------------------ */
/*  Utility: push a value into the cache, resizing if necessary.    */
/* ------------------------------------------------------------------ */
static void push_cache(seq_t val)
{
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_size == cache_cap) {
        cache_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (tmp == NULL)
            return;  /* allocation failure – in practice this should not happen */
        cache = tmp;
    }
    cache[cache_size++] = val;
}

/* ------------------------------------------------------------------ */
/*  Sum of the digits of n in factorial (factoradic) representation.   */
/*  n >= 1.  The result fits easily in seq_t for the required range.  */

/* ------------------------------------------------------------------ */
/*  Ensure that the cache contains at least n terms.  Enumerates     */
/*  integers, tests each with the required property, and stores     */
/*  those that satisfy it.  This is independent of order of calls.   */
/* ------------------------------------------------------------------ */
static void ensure_terms(idx_t n)
{
    while (cache_size < n) {
        seq_t val = next_test;
        seq_t sum = sum_fact_base_digits(val);
        if (gcd(val, sum) == 1)
            push_cache(val);
        next_test++;  /* move to next integer */
    }
}
