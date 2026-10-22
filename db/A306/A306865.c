/*
 * A306865  Lexicographically earliest sequence of distinct positive terms
 *          such that the binary representation of the sum of two consecutive
 *          terms has at least 5 runs of ones.
 * https://oeis.org/A306865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_at_least_runs, used_check
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Lexicographically earliest sequence of distinct positive terms such that
 *  the binary representation of the sum of two consecutive terms has at
 *  least five runs of ones.  The sequence is constructed greedily:
 *  a(1) = 1, and for n > 1, a(n) is the smallest unused positive integer
 *  x for which bin(a(n-1) + x) contains at least five runs of consecutive
 *  ones.  The implementation stores already computed terms in a static
 *  array and remembers which integers have been used via a dynamic bitset.
 *  The function works for any seq_t/idx_t size supported by the build and
 *  can be called in any order. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/has_at_least_runs.h"
#include "../utils/used_check.h"

/* ensures the bitset can hold at least 'v' and zeros new bits */
static void ensure_used_cap(idx_t v, unsigned char **used, idx_t *used_cap)
{
    if (v <= *used_cap)
        return;
    idx_t old_cap = *used_cap;
    idx_t new_cap = (old_cap == 0) ? 1 : old_cap << 1;
    while (new_cap < v)
        new_cap <<= 1;
    idx_t old_bytes = (old_cap >> 3) + 1;
    idx_t new_bytes = (new_cap >> 3) + 1;
    unsigned char *tmp = (unsigned char *)realloc(*used, new_bytes);
    if (!tmp)
        abort();   /* out of memory */
    *used = tmp;
    /* zero new bytes */
    if (new_bytes > old_bytes)
        memset(*used + old_bytes, 0, new_bytes - old_bytes);
    *used_cap = new_cap;
}

/* main function implementing the sequence */
seq_t a(idx_t n)
{
    /* static storage for computed terms and bitset of used indices */
    static seq_t *seq            = NULL;
    static idx_t seq_len         = 0;
    static size_t seq_cap        = 0;
    static unsigned char *used   = NULL;
    static idx_t used_cap        = 0;

    /* lazy initialization */
    if (seq_len == 0) {
        /* start with a(1) = 1 */
        seq_cap = 128;
        seq     = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq)
            abort();
        seq[0]   = (seq_t)1;
        seq_len  = 1;
        /* initialize used bitset */
        fputs("", NULL); /* dummy to keep style uniform */
        ensure_used_cap(1, &used, &used_cap);
        used[1 >> 3] |= 1 << (1 & 7);
    }

    /* compute terms up to n if necessary */
    while (seq_len < n) {
        seq_t prev = seq[seq_len - 1];
        /* search for smallest unused candidate */
        idx_t candidate = 1;
        while (true) {
            if (used_check(candidate, used, used_cap))
                ++candidate;                /* already used */
            else {
                seq_t sum = prev + (seq_t)candidate;
                if (has_at_least_runs(sum, 5))
                    break;                  /* found suitable candidate */
                ++candidate;                /* try next number */
            }
        }
        /* grow seq array if needed */
        if (seq_len >= seq_cap) {
            size_t new_cap = seq_cap << 1;
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp)
                abort();
            seq = tmp;
            seq_cap = new_cap;
        }
        /* store new term */
        seq[seq_len] = (seq_t)candidate;
        ++seq_len;

        /* update used bitset */
        ensure_used_cap(candidate, &used, &used_cap);
        used[candidate >> 3] |= 1 << (candidate & 7);
    }

    return seq[n - 1];
}
