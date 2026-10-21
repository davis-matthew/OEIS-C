/*
 * A249721  Numbers whose base-3 representation consists entirely of 0's and
 *          2's, except possibly for a single pair of adjacent 1's among them.
 * https://oeis.org/A249721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 3327, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumeration of numbers whose base‑3 representation consists entirely
 * of 0's and 2's, except possibly for a single pair of adjacent 1's
 * among them.  The function generates sequence terms on demand by
 * scanning natural numbers, testing the string condition, and storing
 * them in a static cache so that calls with arbitrary ordering of n
 * are fast. */
#include <stdlib.h>

static seq_t *cache = NULL;          /* dynamic array of sequence terms */
static size_t cache_size = 0;        /* number of terms stored */
static size_t cache_cap   = 0;        /* allocated capacity of cache   */
static seq_t next_value = (seq_t)0;  /* next integer to test          */

/* Return 1 if x satisfies the digit condition, 0 otherwise.  */
static int is_good(seq_t x)
{
    if (x == 0)
        return 1;                     /* 0 is the first term a(0) */

    seq_t t = x;
    int pos = 0;
    int ones[64];
    int ones_count = 0;

    /* Extract base‑3 digits (least–significant first). */
    for (; t > 0; t /= (seq_t)3, pos++) {
        seq_t d = t % (seq_t)3;
        if (d == (seq_t)1) {
            if (ones_count < 64)
                ones[ones_count] = pos;
            ones_count++;
        }
    }

    if (ones_count == 0)
        return 1;                      /* only 0’s and 2’s           */
    if (ones_count == 2 && ones[0] + 1 == ones[1])
        return 1;                      /* exactly one adjacent pair  */
    return 0;
}

/* Append a new term to the dynamic cache. */
static void push_seq(seq_t val)
{
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        seq_t *p = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache = p;
        cache_cap = new_cap;
    }
    cache[cache_size++] = val;
}

/* Return the n‑th term of sequence A249721 (0‑based indexing). */
seq_t a(idx_t n)
{
    while (cache_size <= n) {
        if (is_good(next_value))
            push_seq(next_value);
        next_value = next_value + (seq_t)1;
    }
    return cache[n];
}
