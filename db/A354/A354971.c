/*
 * A354971  a(1)=1, a(2)=0; for n > 2, a(n) is the number of times a(n - 1 -
 *          a(n-1)) has appeared in the sequence.
 * https://oeis.org/A354971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 0, for n > 2, a(n) = number of times a(n‑1 ­-
 *            a(n‑1)) has appeared in the sequence (i.e. in a(1)..a(n‑1))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

#ifndef __cplusplus
/*<<C mode>>*/
static seq_t *seq          = NULL;
static size_t seq_cap      = 0;
static size_t seq_len      = 0;

static void grow_seq(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 2;
    while (new_cap < need) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (seq == NULL) abort();
    seq_cap = new_cap;
}
#else
/*<<C++ mode>>*/
#include <new>
static seq_t *seq          = nullptr;
static size_t seq_cap      = 0;
static size_t seq_len      = 0;

static void grow_seq(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 2;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_mem = new seq_t[new_cap];
    if (seq != nullptr) {
        for (size_t i = 0; i <= seq_len; ++i) new_mem[i] = seq[i];
        delete [] seq;
    }
    seq = new_mem;
    seq_cap = new_cap;
}
#endif

/* Frequency tables: distinct values and their counts */
#ifndef __cplusplus
static seq_t *freq_keys    = NULL;
#else
static seq_t *freq_keys    = nullptr;
#endif
static size_t *freq_counts = NULL;
static size_t freq_cap     = 0;
static size_t freq_len     = 0;

#ifndef __cplusplus
static void grow_freq(void)
{
    if (freq_cap) freq_cap <<= 1;
    else           freq_cap = 2;
    freq_keys   = (seq_t *)realloc(freq_keys,   freq_cap * sizeof(seq_t));
    freq_counts = (size_t *)realloc(freq_counts, freq_cap * sizeof(size_t));
    if (freq_keys == NULL || freq_counts == NULL) abort();
}
#else
static void grow_freq(void)
{
    if (freq_cap) freq_cap <<= 1;
    else           freq_cap = 2;
    seq_t *new_keys = new seq_t[freq_cap];
    if (freq_keys != nullptr) {
        for (size_t i = 0; i < freq_len; ++i) new_keys[i] = freq_keys[i];
        delete [] freq_keys;
    }
    freq_keys = new_keys;
    size_t *new_counts = new size_t[freq_cap];
    if (freq_counts != nullptr) {
        for (size_t i = 0; i < freq_len; ++i) new_counts[i] = freq_counts[i];
        delete [] freq_counts;
    }
    freq_counts = new_counts;
}
#endif

static int freq_find_index(seq_t val)
{
    for (size_t i = 0; i < freq_len; ++i)
        if (freq_keys[i] == val) return (int)i;
    return -1;
}

static size_t freq_get_count(seq_t val)
{
    int idx = freq_find_index(val);
    return (idx == -1) ? 0 : freq_counts[(size_t)idx];
}

static void freq_inc_count(seq_t val)
{
    int idx = freq_find_index(val);
    if (idx == -1) {
        if (freq_len >= freq_cap) grow_freq();
        freq_keys[freq_len] = val;
        freq_counts[freq_len] = 1;
        ++freq_len;
    } else {
        ++freq_counts[(size_t)idx];
    }
}

static void seq_append(seq_t val)
{
    ++seq_len;
    grow_seq(seq_len + 1);   /* keep a spare slot at seq[0] */
    seq[seq_len] = val;
    freq_inc_count(val);
}

/* Initialise sequence with a(1)=1, a(2)=0 */
static void seq_init(void)
{
    grow_seq(4);          /* capacity at least 3 (index 0 unused) */
    seq_len = 2;
    seq[1]  = (seq_t)1;
    seq[2]  = (seq_t)0;

    grow_freq();
    freq_keys[0]   = seq[1];
    freq_counts[0] = 1;
    freq_keys[1]   = seq[2];
    freq_counts[1] = 1;
    freq_len = 2;
}

/* Ensure the sequence is computed up to index n */
static void seq_ensure_up_to(idx_t n)
{
    if (seq == NULL) seq_init();

    while (seq_len < (size_t)n) {
        /* previous term a(seq_len) */
        idx_t prev = (idx_t)seq[seq_len];
        /* index k = n-1 - a(n-1) = seq_len - prev */
        idx_t k = (idx_t)seq_len - prev;
        /* target value a(k) */
        seq_t target = seq[(size_t)k];
        /* count of target up to seq_len (i.e. up to n-1) */
        size_t count = freq_get_count(target);
        /* append next term */
        seq_append((seq_t)count);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined before offset */
    seq_ensure_up_to(n);
    return seq[(size_t)n];
}
