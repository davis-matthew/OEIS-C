/*
 * A258209  Numbers k for which A256999(A059893(k)) = k.
 * https://oeis.org/A258209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 892, u32 n <= 5117, u64 n <= 5117,
 *            u128 n <= 5117, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: enumerate integers k and test the binary‑necklace
   symmetry conditions that characterize A258209.  Uses 64‑bit
   arithmetic internally; values are cast to the numeric type seq_t
   for storage. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Reverse the lowest `len` bits of `x`. */
static uint64_t reverse_bits(uint64_t x, int len)
{
    uint64_t rev = 0;
    for (int i = 0; i < len; ++i) {
        if ((x >> i) & 1ULL)
            rev |= 1ULL << (len - 1 - i);
    }
    return rev;
}

/* Rotate the lowest `len` bits of `x` left by `s` positions. */
static uint64_t rotate_left(uint64_t x, int s, int len)
{
    if (len == 0) return 0;
    if (s == 0) return x & ((len == 64) ? ~0ULL : ((1ULL << len) - 1));

    uint64_t mask = (len == 64) ? ~0ULL : ((1ULL << len) - 1);
    return ((x << s) | (x >> (len - s))) & mask;
}

/* Test whether a given unsigned integer k satisfies the conditions
   of A258209. This follows the definition:
   - k must be lexicographically largest among its rotations.
   - Let t be k without the most significant 1 bit (len-1 bits).
   - t must be a rotation of its bit‑wise reverse (within len-1 bits).
   - t must itself be lexicographically largest among its rotations. */
static bool is_in_sequence(uint64_t k)
{
    if (k == 0) return true;          /* special case a(0) = 0 */

    /* Bit length of k. */
    int len = 64 - __builtin_clzll(k);

    /* 1. k must be the lexicographically largest rotation of itself. */
    bool kmax = true;
    for (int s = 1; s < len && kmax; ++s) {
        uint64_t rot = rotate_left(k, s, len);
        if (rot > k) kmax = false;
    }
    if (!kmax) return false;

    /* 2. Extract t (k without the leading 1). Length len_t = len-1. */
    int len_t = len - 1;
    if (len_t <= 0) return true;      /* k=1 or k=0 already handled */

    uint64_t hp = 1ULL << len_t;
    uint64_t t = k - hp;              /* t uses the lower len_t bits */

    /* 3. t must be a rotation of its reverse within len_t bits. */
    uint64_t rev_t = reverse_bits(t, len_t);
    bool rev_sym = false;
    for (int s = 0; s < len_t; ++s) {
        uint64_t rot_rev = rotate_left(rev_t, s, len_t);
        if (rot_rev == t) { rev_sym = true; break; }
    }
    if (!rev_sym) return false;

    /* 4. t must be the lexicographically largest rotation of itself. */
    bool tmax = true;
    for (int s = 1; s < len_t && tmax; ++s) {
        uint64_t rot_t = rotate_left(t, s, len_t);
        if (rot_t > t) tmax = false;
    }
    return tmax;
}

/* Dynamic array state for memoising sequence values. */
static seq_t *seq_vals = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;
static idx_t next_k    = 0;

/* Append a value to the dynamic array. */
static void push_val(seq_t val)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 4;
        seq_vals = (seq_t *)realloc(seq_vals, seq_cap * sizeof(seq_t));
    }
    seq_vals[seq_len++] = val;
}

/* Return the n‑th term of the sequence (zero‑based indexing). */
seq_t a(idx_t n)
{
    while (seq_len <= n) {
        uint64_t k = (uint64_t)next_k;
        if (is_in_sequence(k))
            push_val((seq_t)k);
        ++next_k;
    }
    return seq_vals[n];
}
