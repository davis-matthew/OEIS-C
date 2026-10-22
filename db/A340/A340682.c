/*
 * A340682  The closure under squaring of the nonunit squarefree numbers.
 * https://oeis.org/A340682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   factor_u64, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A340682: the closure under squaring of the nonunit squarefree numbers.
    a(n) is the nth positive integer m>1 such that in the prime factorization
    all prime exponents are equal to a power of two (i.e. m = s^(2^e) with
    s a squarefree integer >1).  The sequence starts with 2,3,4,5,6,7,9,...   */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* static cache for the sequence values */
static seq_t *seq_cache = NULL;   /* array of sequence values           */
static size_t   seq_cap   = 0;     /* allocated capacity                 */
static size_t   seq_len   = 0;     /* number of stored values            */
static uint64_t last_checked = 1;  /* largest integer already examined   */

/* Ensure the cache can hold at least 'needed' elements */
static void ensure_capacity(size_t needed)
{
    if (seq_cap >= needed)
        return;
    size_t newcap = seq_cap ? seq_cap * 2 : 128;
    while (newcap < needed)
        newcap <<= 1;
    seq_cache = seq_realloc(seq_cache, seq_cap, newcap);
    seq_cap   = newcap;
}

/* Test whether m>1 is of the form s^(2^e) with s squarefree  */
static bool is_closure_member(uint64_t m)
{
    if (m < 2)
        return false;
    /* factorize m */
    size_t   cnt;
    uint64_t pr[64], ex[64];
    factor_u64(m, pr, ex, &cnt);
    if (cnt == 0)
        return false;
    uint64_t e = ex[0];
    for (size_t i = 1; i < cnt; ++i)
        if (ex[i] != e)
            return false;
    /* e must be a power of two (1,2,4,8,...) */
    return (e & (e - 1)) == 0;
}

/* Generate sequence values up to at least index n */
static void generate_up_to(idx_t n)
{
    if (seq_len >= n)
        return;
    /* allocate initial storage */
    if (seq_cache == NULL) {
        seq_cap   = 128;
        seq_cache = seq_alloc(seq_cap);
        seq_len   = 0;
    }
    uint64_t candidate = last_checked + 1;
    while (seq_len < (size_t)n) {
        if (is_closure_member(candidate)) {
            ensure_capacity(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)candidate;
        }
        ++candidate;
    }
    last_checked = candidate - 1;
}

seq_t a(idx_t n)
{
    generate_up_to(n);
    return seq_cache[n - 1];
}
