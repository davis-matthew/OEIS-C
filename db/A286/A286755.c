/*
 * A286755  Möbius (or Moebius) partition function of partitions in graded
 *          reverse lexicographic ordering.
 * https://oeis.org/A286755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = μP(p_n) where p_n is the n‑th partition ordered by
 *            increasing sum, and within each sum by reverse lexicographic
 *            order. μP(p) = (-1)^k if p has k distinct parts, otherwise μP(p)
 *            = 0
 * Range:     i8 n <= 84, i16 n <= 84, i32 n <= 84, i64 n <= 84, i128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- dynamic array handling (compatible with C and C++) ---------- */
static seq_t *cache     = NULL;   /* values of a(k), k>=1 stored 0‑based */
static size_t cache_len = 0;      /* number of values stored */
static size_t cache_cap = 0;      /* allocated capacity */

/* increase capacity to at least 'needed' */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
    while (new_cap < needed) new_cap *= 2;

#ifdef __cplusplus
    /* allocate new array and move existing values */
    seq_t *new_arr = new seq_t[new_cap];
    for (size_t i = 0; i < cache_len; ++i) new_arr[i] = cache[i];
    delete[] cache;
    cache = new_arr;
#else
    /* C allocation via realloc */
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    cache = new_arr;
#endif
    cache_cap = new_cap;
}

/* append a new value to the cache */
static void append_to_cache(seq_t value)
{
    ensure_capacity(cache_len + 1);
    cache[cache_len++] = value;
}

/* ---------- partition generation in graded reverse lexicographic order ---------- */
static void process_partition(idx_t depth, const idx_t *parts)
{
    bool repeated = false;
    idx_t distinct = 0;
    if (depth > 0) {
        distinct = 1;                /* first part is distinct */
        for (idx_t i = 1; i < depth; ++i) {
            if (parts[i] == parts[i-1]) {
                repeated = true;
                break;
            }
            ++distinct;
        }
    }
    seq_t mu;
    if (repeated) {
        mu = (seq_t)0;
    } else {
        mu = (distinct % 2 == 0) ? (seq_t)1 : (seq_t)(-1);
    }
    append_to_cache(mu);
}

static void gen_partitions_rec(idx_t remaining, idx_t max_part,
                               idx_t depth, idx_t *parts)
{
    if (remaining == 0) {
        process_partition(depth, parts);
        return;
    }
    for (idx_t part = (remaining < max_part ? remaining : max_part);
         part >= 1; --part) {
        parts[depth] = part;
        gen_partitions_rec(remaining - part, part, depth + 1, parts);
    }
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1 but safe */

    /* current sum to generate partitions for */
    static idx_t next_sum = 1;

    /* generate partitions until we have at least n values */
    if (cache_len < (size_t)n) {
        static idx_t parts[512];   /* depth will never exceed this for usual n */
        while (cache_len < (size_t)n) {
            gen_partitions_rec(next_sum, next_sum, 0, parts);
            ++next_sum;
        }
    }
    return cache[n - 1];
}
