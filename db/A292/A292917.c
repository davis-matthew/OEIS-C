/*
 * A292917  Numbers n for which the n-th row of A008284 (partitions of n into
 *          k parts) has duplicate values > 1.
 * https://oeis.org/A292917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A292917
 *  a(n) = the n-th number N such that the N-th row of the partition
 *  table A008284 contains a duplicated value >1.
 *
 *  The sequence consists of all numbers of the form 2·P(k) or
 *  2·P(k)+1 (k ≥ 2, with P(k) the partition number) together with the
 *  special values {8,13,19,26,34,43,46,68}.  The terms are returned
 *  in increasing order.
 */

#include <stdbool.h>
#include <stdlib.h>

/* ------------  partition numbers P(k)  ------------ */
static seq_t *partitions = NULL;
static size_t part_cap   = 0;
static size_t part_len   = 0;   /* how many are currently computed (next free index) */

/* ensure partitions array has space for (idx_t)max_index and compute all new values */
static void ensure_partitions(idx_t max_index)
{
    if (partitions == NULL) {
        /* silence compiler warnings */
        part_cap = 16;
        partitions = (seq_t*)calloc(part_cap, sizeof(seq_t));
        if (!partitions) { exit(1); }
        /* base cases */
        partitions[0] = (seq_t)1;
        partitions[1] = (seq_t)1;
        part_len = 2;
    }
    if (part_len > (size_t)max_index) return;

    /* expand capacity if needed */
    if (part_cap <= (size_t)max_index) {
        size_t new_cap = part_cap;
        while (new_cap <= (size_t)max_index) new_cap <<= 1;
        partitions = (seq_t*)realloc(partitions, new_cap * sizeof(seq_t));
        part_cap   = new_cap;
    }

    /* compute missing partition numbers */
    for (idx_t n = part_len; n <= max_index; ++n) {
        seq_t sum = 0;
        for (idx_t k = 1;; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > n) break;
            seq_t term = partitions[n - g1];
            if (k & 1) sum += term; else sum -= term;
            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 > n) continue;
            term = partitions[n - g2];
            if (k & 1) sum += term; else sum -= term;
        }
        partitions[n] = sum;
    }
    part_len = (size_t)max_index + 1;
}

/* ------------  sequence storage  ------------ */
static seq_t *seq_arr   = NULL;
static size_t seq_cap   = 0;
static size_t seq_len   = 0;
static size_t seq_max_n = 0;          /* largest n for which a(n) is precomputed */

/* special numbers that are not of the form 2·P(k) or 2·P(k)+1 */
static const seq_t specials[] = { 8,13,19,26,34,43,46,68 };
static const size_t specials_len = sizeof(specials)/sizeof(specials[0]);

/* indices controlling generation of 2·P(k) and 2·P(k)+1 */
static idx_t k_v = 2;      /* next k for value 2·P(k)+1   (k >= 2) */
static idx_t k_u = 3;      /* next k for value 2·P(k)     (k >= 3) */
static size_t special_idx = 0;

/* make sure the array can hold at least need elements */
static void ensure_seq_cap(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_arr = (seq_t*)realloc(seq_arr, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* generate terms until we have at least count elements */
static void generate_terms(size_t count)
{
    ensure_seq_cap(count);
    while (seq_len < count) {
        /* compute candidates */
        seq_t val_v = (seq_t)-1, val_u = (seq_t)-1, next_sp = (seq_t)-1;
        bool have_v = (k_v > 0);
        bool have_u = (k_u > 0);
        bool have_s = (special_idx < specials_len);

        if (have_v) {
            ensure_partitions(k_v);
            val_v = (seq_t)2 * partitions[k_v] + 1;
        }
        if (have_u) {
            ensure_partitions(k_u);
            val_u = (seq_t)2 * partitions[k_u];
        }
        if (have_s) next_sp = specials[special_idx];

        /* pick the smallest available candidate */
        seq_t candidate = 0;
        bool first = true;
        if (have_s) { candidate = next_sp; first = false; }
        if (have_v && (first || val_v < candidate)) { candidate = val_v; first = false; }
        if (have_u && (first || val_u < candidate)) { candidate = val_u; first = false; }

        /* store it */
        seq_arr[seq_len++] = candidate;

        /* advance the source that produced it */
        if (have_s && candidate == next_sp) {
            ++special_idx;
        } else if (have_v && candidate == val_v) {
            ++k_v;
        } else {
            ++k_u;
        }
    }
}

/* ------------  public interface  ------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    generate_terms((size_t)n);   /* guarantees seq_len >= n */
    return seq_arr[n - 1];
}
