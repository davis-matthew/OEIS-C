/*
 * A256696  R(k), the minimal alternating binary representation of k,
 *          concatenated for k = 0, 1, 2,....
 * https://oeis.org/A256696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, base
 * Formula:   not stated
 * Range:     i8 n <= 193, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* 
 * Formula: a(n) = concatenation of the minimal alternating binary 
 * representation of n, for n = 0, 1, 2, …, with a(0) = 0.
 * The representation of a positive integer k is obtained by the
 * recurrence:
 *   if k == 0            : [0]
 *   if k is a power of 2 : [k]
 *   otherwise             : [p] + (- representation of (p - k))
 *   where p = the smallest power of two >= k.
 */

static idx_t
next_power_of_two(idx_t n)
{
    if (n == 0)
        return (idx_t)1;
    if ((n & (n - 1)) == 0)       /* already a power of two */
        return n;
    idx_t v = 1;
    while (v < n)
        v <<= 1;
    return v;
}

/* cache for lengths of representations: len_cache[k] = len_of_k(k) */
static size_t *len_cache = NULL;      /* array of len_cache_cap entries */
static size_t len_cache_cap = 0;

/* recursive length of the representation of k with memoisation */
static size_t
len_of_k(idx_t k)
{
    if (k == 0)
        return (size_t)1;                  /* [0] */
    if ((k & (k - 1)) == 0)
        return (size_t)1;                  /* [k] for powers of two */

    if ((size_t)k < len_cache_cap && len_cache[(size_t)k] != 0)
        return len_cache[(size_t)k];

    /* ensure sufficient storage */
    if ((size_t)k >= len_cache_cap) {
        size_t new_cap = len_cache_cap == 0 ? 16 : len_cache_cap;
        while (new_cap <= (size_t)k)
            new_cap <<= 1;
        size_t *tmp = (size_t *)realloc(len_cache, new_cap * sizeof(size_t));
        if (!tmp)
            exit(1);
        for (size_t i = len_cache_cap; i < new_cap; ++i)
            tmp[i] = 0;
        len_cache = tmp;
        len_cache_cap = new_cap;
    }

    idx_t p = next_power_of_two(k);
    idx_t m = p - k;
    size_t len = 1 + len_of_k(m);
    len_cache[(size_t)k] = len;
    return len;
}

/* recursive extraction of the offset-th term of the representation of k */
static seq_t
term_of_k(idx_t k, size_t offset)
{
    if (k == 0)
        return (seq_t)0;                     /* representation [0] */
    seq_t p = (seq_t)next_power_of_two(k);
    if (offset == 0)
        return p;
    idx_t m = (idx_t)(p) - k;                /* since p >= k and p is power of two */
    seq_t val = term_of_k(m, offset - 1);
    return -val;
}

/* ------------------------------------------------------------------ */
/*  Public function implementing OEIS sequence A256696               */
/* ------------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    idx_t cumulative = 0;
    idx_t k = 0;

    while (1) {
        size_t len_k = len_of_k(k);
        if (cumulative + (idx_t)len_k > n) {
            size_t offset = (size_t)(n - cumulative);
            return term_of_k(k, offset);
        }
        cumulative += (idx_t)len_k;
        ++k;
    }
}
