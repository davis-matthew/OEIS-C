/*
 * A330708  Numbers that are not the sum of 2 nonzero squares and a positive
 *          cube.
 * https://oeis.org/A330708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 396, u32 n <= 490, u64 n <= 490,
 *            u128 n <= 490, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers that are not the sum of 2 nonzero squares and a positive cube.
   a(n) = the n‑th nonnegative integer that cannot be written as
   a^2 + b^2 + c^3 with a,b > 0 and c > 0. */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* Static tables for fast look‑ups of sums of two nonzero squares. */
static uint64_t *sum_bits = NULL;   /* bit i set iff i = a^2 + b^2 (a,b>0) */
static idx_t sum_limit = 0;         /* largest value represented */
static size_t sum_words = 0;        /* number of 64‑bit words */

/* Test whether v can be expressed as the sum of two nonzero squares. */
static inline int is_sum_of_two_squares(idx_t v)
{
    if (v > sum_limit) return 0;
    idx_t idx  = v >> 6;
    uint64_t mask = 1ULL << (v & 63);
    return (sum_bits[idx] & mask) != 0;
}

/* Set the bit corresponding to value v. */
static inline void set_sum_bit(idx_t v)
{
    idx_t idx  = v >> 6;
    uint64_t mask = 1ULL << (v & 63);
    sum_bits[idx] |= mask;
}

/* Pre‑compute all a² + b² (a,b>0) up to LIMIT. */
static void precompute_sums(idx_t LIMIT)
{
    if (sum_bits) return;                /* already done */

    sum_limit = LIMIT;
    sum_words = (LIMIT >> 6) + 1;
    sum_bits = (uint64_t *)calloc(sum_words, sizeof(uint64_t));
    if (!sum_bits) abort();

    idx_t a_max = (idx_t)sqrt((double)LIMIT);
    for (idx_t a = 1; a <= a_max; ++a) {
        idx_t a2 = a * a;
        idx_t rem = LIMIT - a2;
        idx_t b_max = (idx_t)sqrt((double)rem);
        for (idx_t b = 1; b <= b_max; ++b) {
            set_sum_bit(a2 + b * b);
        }
    }
}

/* Sequence cache. */
static seq_t *seq_cache = NULL;          /* stored a(k) values */
static size_t seq_cap   = 0;             /* allocated capacity */
static size_t seq_len   = 0;             /* number of elements stored */
static idx_t next_candidate = 0;         /* next integer to test */

/* Append value to cache, expanding the array if necessary. */
static void cache_append(seq_t v)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 16;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_cache = tmp;
        seq_cap   = new_cap;
    }
    seq_cache[seq_len++] = v;
}

/* Integer cube root: floor(cbrt(n)).  Works for n of type idx_t. */
static idx_t icbrt(idx_t n)
{
    /* Initial guess via floating point to keep loop short. */
    idx_t low  = 0;
    idx_t high = (idx_t)pow((double)n, 1.0 / 3.0) + 2;
    while ((high + 1ULL) * (high + 1ULL) * (high + 1ULL) <= n) ++high;
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if (mid * mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Test whether v can be written as a^2 + b^2 + c^3 with a,b>0, c>0. */
static int is_representable(idx_t v)
{
    if (v < 3) return 0;               /* 1^2+1^2+1^3 = 3 minimal */
    idx_t c_max = icbrt(v);
    for (idx_t c = 1; c <= c_max; ++c) {
        idx_t cube = c * c * c;
        idx_t rem  = v - cube;
        if (rem < 2) continue;         /* need at least 1^2+1^2 = 2 */
        if (is_sum_of_two_squares(rem))
            return 1;
    }
    return 0;
}

/* Public entry point. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined: guard against bad callers */

    /* Initialise once.  The chosen bound is generous for n ≤ 10000. */
    if (!sum_bits) {
        precompute_sums(250000000ULL);
    }

    /* Return cached value if already computed. */
    if (n <= seq_len)
        return seq_cache[n - 1];

    /* Generate subsequent terms until we reach the desired index. */
    while (seq_len < n) {
        if (!is_representable(next_candidate))
            cache_append((seq_t)next_candidate);
        ++next_candidate;
    }
    return seq_cache[n - 1];
}
