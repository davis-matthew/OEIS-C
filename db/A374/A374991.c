/*
 * A374991  Lexicographically earliest sequence of distinct nonnegative
 *          integers such that the concatenation of the binary expansions of
 *          two consecutive terms, in at least one way, and after removal of
 *          trailing digits, is palindromic.
 * https://oeis.org/A374991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Greedy construction: a(0)=0, a(1)=1. For n≥2, a(n) is the
   smallest non‑negative integer not yet used that together with
   a(n‑1) yields a binary palindrome after concatenating their
   binary representations in either order and stripping all
   trailing zeros.  */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/grow_seq.h"

static int initialized = 0;
static seq_t *sequence  = NULL;
static size_t seq_cap   = 0;
static size_t seq_len   = 0;
static seq_t *used      = NULL;
static size_t used_cap  = 0;
static size_t used_len  = 0;

/* Return the number of bits required to represent x (≥1). */
static int bitlen(seq_t x)
{
    if (x == 0) return 1;
    int len = 0;
    seq_t t = x;
    while (t != 0) {
        ++len;
        t >>= 1;
    }
    return len;
}

/* Check whether x (assumed >0) is a binary palindrome. */
static bool is_binary_palindrome(seq_t x)
{
    if (x == 0) return true;  /* binary "0" is a palindrome */
    int len = bitlen(x);
    for (int i = 0; i < len / 2; ++i) {
        bool low  = ((x >> (idx_t)i) & (seq_t)1) != 0;
        bool high = ((x >> (idx_t)(len - 1 - i)) & (seq_t)1) != 0;
        if (low != high) return false;
    }
    return true;
}

/* Return true if z, after removing all trailing zeros, is a binary
   palindrome. */
static bool palindromic_odd(seq_t z)
{
    seq_t odd = z;
    while ((odd & (seq_t)1) == 0) odd >>= 1;   /* remove trailing zeros */
    return is_binary_palindrome(odd);
}

/* Does the pair (prev,cand) satisfy the concatenation condition? */
static bool satisfies(seq_t prev, seq_t cand)
{
    seq_t z1 = (prev << (idx_t)bitlen(cand)) | cand;   /* prev first */
    if (palindromic_odd(z1)) return true;
    seq_t z2 = (cand << (idx_t)bitlen(prev)) | prev;   /* cand first */
    return palindromic_odd(z2);
}

/* Return true if val appears in the used array. */
static bool used_contains(seq_t val)
{
    for (size_t i = 0; i < used_len; ++i)
        if (used[i] == val) return true;
    return false;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        /* initial allocation */
        grow_seq(&sequence, &seq_cap, 128);
        grow_seq(&used,      &used_cap, 128);

        sequence[0] = 0;
        sequence[1] = 1;
        seq_len = 2;

        used[0] = 0;
        used[1] = 1;
        used_len = 2;

        initialized = 1;
        if (n == 0) return 0;
        if (n == 1) return 1;
    }

    /* generate terms up to n if needed */
    while (seq_len <= (size_t)n) {
        seq_t prev = sequence[seq_len - 1];
        seq_t cand = 0;

        /* find next candidate */
        for (;;) {
            if (used_contains(cand)) { cand++; continue; }
            if (satisfies(prev, cand)) break;
            cand++;
        }

        /* append to sequence */
        if (seq_len == seq_cap) grow_seq(&sequence, &seq_cap, seq_len + 1);
        sequence[seq_len] = cand;
        ++seq_len;

        /* mark as used */
        if (used_len == used_cap) grow_seq(&used, &used_cap, used_len + 1);
        used[used_len++] = cand;
    }

    return sequence[n];
}
