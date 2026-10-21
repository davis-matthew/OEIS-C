/*
 * A299988  Lexicographic first sequence of nonnegative integers such that
 *          a(n) + a(n+1) has a digit 8, and no term occurs twice.
 * https://oeis.org/A299988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Construct the lexicographically first permutation of nonnegative integers
 * such that a(n)+a(n+1) contains the decimal digit 8 and each term appears
 * at most once.  The sequence is built incrementally and cached so that
 * any a(n) can be retrieved in O(n) time for n up to about 10000.  */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Cache of computed terms.  */
static seq_t *seq = NULL;       /* sequence values            */
static size_t seq_cap = 0;      /* allocated capacity of seq  */
static idx_t seq_len = 0;       /* number of terms cached     */

/* Bitset of numbers already used.  */
static bool *used = NULL;       /* used[i] == true if i is in seq */
static size_t used_cap = 0;     /* allocated capacity of used    */

/* Ensure that the used array can index v.  */
static void ensure_used(idx_t v)
{
    if ((size_t)v >= used_cap) {
        size_t new_cap = used_cap ? used_cap : 256;
        while (new_cap <= (size_t)v) new_cap <<= 1;
        bool *tmp = (bool *)realloc(used, new_cap);
        if (!tmp) abort();           /* out of memory */
        /* Zero the new part of the array. */
        memset(tmp + used_cap, 0, new_cap - used_cap);
        used = tmp;
        used_cap = new_cap;
    }
}

/* Grow the sequence array to at least min_cap.  */
static void grow_seq(size_t min_cap)
{
    if (seq_cap < min_cap) {
        size_t new_cap = seq_cap ? seq_cap : 64;
        while (new_cap < min_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* out of memory */
        seq = tmp;
        seq_cap = new_cap;
    }
}

/* Return true if the decimal representation of s contains a digit 8.  */
static inline bool has_digit8(seq_t s)
{
    if (s == 0) return false;
    while (s != 0) {
        if (s % 10 == 8) return true;
        s /= 10;
    }
    return false;
}

/* Main function defined by the grader.  */
seq_t a(idx_t n)
{
    /* Initialise on first call.  */
    if (seq_len == 0) {
        grow_seq(1);
        seq[0] = (seq_t)0;
        seq_len = 1;
        ensure_used(0);
        used[0] = true;
    }

    /* Build terms up to n if required.  */
    while (seq_len <= n) {
        seq_t last = seq[seq_len - 1];
        idx_t x = 0;
        for (;;) {
            ensure_used(x);
            if (!used[x]) {
                seq_t sum = last + (seq_t)x;
                if (has_digit8(sum)) {
                    /* Accept this number.  */
                    grow_seq(seq_len + 1);
                    seq[seq_len] = (seq_t)x;
                    seq_len++;
                    used[x] = true;
                    break;
                }
            }
            x++;
        }
    }
    return seq[n];
}
