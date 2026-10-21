/*
 * A263650  A variation on A098550 (the Yellowstone permutation): a(n)=n for 1
 *          <= n <= 3, a(4)=5; otherwise a(n) = smallest number not yet
 *          appearing in the sequence which is coprime to a(n-1) and not
 *          coprime to a(n-2).
 * https://oeis.org/A263650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 102, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A263650: a variation on A098550 (the Yellowstone permutation)
   Recurrence: a(1)=1, a(2)=2, a(3)=3, a(4)=5;
   for n>4, a(n)=smallest unused m such that
   gcd(a(n-1),m)=1  and  gcd(a(n-2),m)>1. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

#ifdef __cplusplus
#include <vector>
#include <set>
#endif

/* Forward declaration of the sequence function */
seq_t a(idx_t n);

/* ------------------------------------------------------------------ */
/* C-only helpers ----------------------------------------------------- */
#ifndef __cplusplus
static seq_t *seq = NULL;          /* array of computed terms                     */
static size_t seq_len = 0;         /* current number of stored terms              */
static size_t seq_cap = 0;         /* allocated capacity of seq                  */

static uint64_t *used_bits = NULL; /* bitset: bit i-1 set => number i used        */
static size_t used_words = 0;      /* number of 64‑bit words in used_bits         */

/* Ensure seq array has capacity for at least needed elements.   */
static inline void ensure_seq_capacity(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < needed)
        new_cap *= 2;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Ensure used_bits covers number x. */
static inline void ensure_used_capacity(seq_t x)
{
    /* x is at least 1. */
    size_t idx = (size_t)(x - 1);           /* 0‑based bit index                     */
    size_t word = idx >> 6;                 /* divide by 64                           */
    if (word < used_words)
        return;
    size_t new_words = used_words ? used_words * 2 : 1;
    while (new_words <= word)
        new_words *= 2;
    uint64_t *new_bits = (uint64_t *)realloc(used_bits, new_words * sizeof(uint64_t));
    if (!new_bits)
        abort();
    /* Zero the newly allocated words */
    for (size_t i = used_words; i < new_words; ++i)
        new_bits[i] = 0;
    used_bits = new_bits;
    used_words = new_words;
}

/* Set the bit for number val.    */
static inline void set_used_bit(seq_t val)
{
    if (val == 0)
        return;              /* we never track 0                            */
    size_t idx = (size_t)(val - 1);
    size_t word = idx >> 6;
    size_t bit  = idx & 63;
    ensure_used_capacity(val);
    used_bits[word] |= ((uint64_t)1 << bit);
}

/* Test whether number val has been used. */
static inline bool is_used_bit(seq_t val)
{
    if (val == 0)
        return false;
    size_t idx = (size_t)(val - 1);
    size_t word = idx >> 6;
    size_t bit  = idx & 63;
    if (word >= used_words)
        return false;
    return (used_bits[word] >> bit) & 1;
}

/* Initialize sequence with the first four terms. */
static inline void init_sequence_c(void)
{
    if (seq_len > 0)
        return;      /* already initialized */

    seq_cap = 16;
    seq = (seq_t *)calloc(seq_cap, sizeof(seq_t));
    seq_len = 4;
    seq[0] = (seq_t)1;
    seq[1] = (seq_t)2;
    seq[2] = (seq_t)3;
    seq[3] = (seq_t)5;

    used_words = 1;
    used_bits = (uint64_t *)calloc(1, sizeof(uint64_t));
    set_used_bit((seq_t)1);
    set_used_bit((seq_t)2);
    set_used_bit((seq_t)3);
    set_used_bit((seq_t)5);
}
#endif /* !__cplusplus */

/* ------------------------------------------------------------------ */
/* C++-only helpers --------------------------------------------------- */
#ifdef __cplusplus
/* The static data are defined only once, even if multiple translation units
   include this file. */
static std::vector<seq_t> seq_vec;
static std::set<seq_t> used_set;     /* stores all used numbers */

static inline void init_sequence_cpp(void)
{
    if (!seq_vec.empty())
        return;     /* already initialized */

    seq_vec.reserve(16);
    seq_vec.push_back((seq_t)1);
    seq_vec.push_back((seq_t)2);
    seq_vec.push_back((seq_t)3);
    seq_vec.push_back((seq_t)5);

    used_set.insert((seq_t)1);
    used_set.insert((seq_t)2);
    used_set.insert((seq_t)3);
    used_set.insert((seq_t)5);
}
#endif /* __cplusplus */

/* ------------------------------------------------------------------ */
/* Public function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;  /* undefined, return 0 for safety */

#ifdef __cplusplus
    init_sequence_cpp();
    /* Return already computed term if available */
    if ((idx_t)seq_vec.size() >= n)
        return seq_vec[n - 1];

    /* Compute terms up to n */
    while ((idx_t)seq_vec.size() < n)
    {
        seq_t prev1 = seq_vec[seq_vec.size() - 1];
        seq_t prev2 = seq_vec[seq_vec.size() - 2];
        seq_t cand = (seq_t)1;
        while (true)
        {
            if (used_set.find(cand) != used_set.end())
            {
                ++cand;
                continue;
            }
            if (gcd(prev1, cand) != (seq_t)1)
            {
                ++cand;
                continue;
            }
            if (gcd(prev2, cand) == (seq_t)1)
            {
                ++cand;
                continue;
            }
            break;  /* cand satisfies conditions */
        }
        seq_vec.push_back(cand);
        used_set.insert(cand);
    }
    return seq_vec[n - 1];
#else
    init_sequence_c();
    /* Return already computed term if available */
    if (seq_len >= n)
        return seq[n - 1];

    /* Compute terms up to n */
    while (seq_len < n)
    {
        seq_t prev1 = seq[seq_len - 1];
        seq_t prev2 = seq[seq_len - 2];
        seq_t cand = (seq_t)1;
        while (true)
        {
            if (is_used_bit(cand))
            {
                ++cand;
                continue;
            }
            if (gcd(prev1, cand) != (seq_t)1)
            {
                ++cand;
                continue;
            }
            if (gcd(prev2, cand) == (seq_t)1)
            {
                ++cand;
                continue;
            }
            break;  /* cand satisfies conditions */
        }
        ensure_seq_capacity(seq_len + 1);
        seq[seq_len] = cand;
        set_used_bit(cand);
        ++seq_len;
    }
    return seq[n - 1];
#endif
}
