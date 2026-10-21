/*
 * A249055  a(1)=0; the next term is always the product of the two smallest
 *          numbers not yet in the sequence and which have not yet been used.
 * https://oeis.org/A249055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A249055 – Product of the two smallest unused natural numbers.
    a(1) = 0.
    For n > 1, let x and y be the two smallest positive integers not yet
    appearing in the sequence. Then a(n) = x * y, and x, y and the product
    are marked as used.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

/* ------------- bitset of “removed” (used) numbers ------------- */
static uint64_t *removed_bits = NULL;   /* each bit represents an integer */
static size_t removed_words = 0;        /* number of uint64_t words used */

/* ensure that removed_bits covers all values up to 'val' */
static void ensure_removed_capacity(idx_t val)
{
    size_t required = (size_t)(val >> 6) + 1;
    if (required <= removed_words) return;

    size_t new_words = removed_words ? removed_words << 1 : 1;
    while (new_words < required) new_words <<= 1;

    uint64_t *tmp = (uint64_t *)realloc(removed_bits,
                                         new_words * sizeof(uint64_t));
    if (!tmp) abort();

    for (size_t i = removed_words; i < new_words; ++i) tmp[i] = 0ULL;
    removed_bits = tmp;
    removed_words = new_words;
}

/* test whether 'val' has already been removed (been used) */
static bool is_removed(idx_t val)
{
    if (val == 0) return false;
    size_t word = (size_t)(val >> 6);
    if (word >= removed_words) return false;
    uint64_t mask = 1ULL << (val & 63);
    return (removed_bits[word] & mask) != 0;
}

/* mark 'val' as removed (used) */
static void mark_removed(idx_t val)
{
    if (val == 0) return;
    ensure_removed_capacity(val);
    size_t word = (size_t)(val >> 6);
    removed_bits[word] |= 1ULL << (val & 63);
}

/* ------------- storage for sequence values ------------- */
static seq_t *seq_cache = NULL;   /* 1‑based cache of computed terms */
static size_t seq_capacity = 0;   /* number of slots currently allocated */
static idx_t seq_len = 0;         /* highest index that has been computed */

/* ensure that seq_cache can hold up to index 'n' (1‑based) */
static void ensure_seq_capacity(idx_t n)
{
    if (seq_cache == NULL) {
#ifdef __cplusplus
        seq_cache = new seq_t[n + 2];
#else
        seq_cache = (seq_t *)malloc((n + 2) * sizeof(seq_t));
#endif
        seq_capacity = n + 2;
    }
    else if ((size_t)n + 2 > seq_capacity) {
        size_t new_cap = seq_capacity * 2;
        if (new_cap < (size_t)n + 2) new_cap = (size_t)n + 2;
#ifdef __cplusplus
        seq_t *tmp = new seq_t[new_cap];
        for (size_t i = 1; i <= seq_len; ++i) tmp[i] = seq_cache[i];
        delete[] seq_cache;
        seq_cache = tmp;
#else
        seq_t *tmp = (seq_t *)realloc(seq_cache,
                                      new_cap * sizeof(seq_t));
        seq_cache = tmp;
#endif
        seq_capacity = new_cap;
    }
}

/* compute terms up to 'n', storing them in seq_cache */
static void compute_until(idx_t n)
{
    if (n <= seq_len) return;        /* already computed */

    ensure_seq_capacity(n);

    static idx_t next_missing = 1;    /* next candidate integer not yet used */

    for (idx_t i = seq_len + 1; i <= n; ++i) {
        if (i == 1) {
            seq_cache[1] = (seq_t)0;
        } else {
            /* find the two smallest remaining missing numbers */
            while (is_removed(next_missing)) ++next_missing;
            idx_t x = next_missing;
            mark_removed(x);
            ++next_missing;

            while (is_removed(next_missing)) ++next_missing;
            idx_t y = next_missing;
            mark_removed(y);

            seq_t prod = (seq_t)x * (seq_t)y;
            seq_cache[i] = prod;

            /* remove the product if it fits in 64 bits */
            if (prod <= (seq_t)ULLONG_MAX)
                mark_removed((idx_t)prod);

            ++next_missing;
        }
    }
    seq_len = n;
}

/* public function required by the problem */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* undefined for n < 1, but safe */
    compute_until(n);
    return seq_cache[n];
}
