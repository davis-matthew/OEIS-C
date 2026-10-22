/*
 * A329450  Lexicographically earliest sequence of distinct nonnegative
 *          integers such that neither a(n) + a(n+1) nor a(n) + a(n+2) is
 *          prime for any n.
 * https://oeis.org/A329450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically earliest permutation of the nonnegative integers such that for all n,
 * the sums a(n)+a(n+1) and a(n)+a(n+2) are never prime.
 * Constructed greedily: at each step pick the smallest unused integer that keeps the two
 * previous pairwise sums composite.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_big.h"   /* provides bool is_prime_big(seq_t) */

/* Dynamic storage for the sequence and a bitset of used integers. */
static seq_t *seq = NULL;          /* sequence values */
static size_t seq_cap = 0;         /* capacity of seq */
static size_t seq_len = 0;         /* number of terms computed */

static unsigned char *used = NULL; /* 0 = unused, 1 = used */
static size_t used_cap = 0;        /* capacity of used */

/* Ensure seq has room for at least 'need' elements. */
static void ensure_seq_cap(size_t need)
{
    if (seq_cap >= need)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Ensure used can index up to 'need' (i.e. indices 0..need-1). */
static void ensure_used_cap(size_t need)
{
    if (used_cap >= need)
        return;
    size_t new_cap = used_cap ? used_cap * 2 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(used, new_cap);
    if (tmp) {
        /* zero newly allocated part */
        memset(tmp + used_cap, 0, new_cap - used_cap);
        used = tmp;
        used_cap = new_cap;
    }
}

/* Build the sequence up to index 'n' (inclusive).  The function
 * accounts for calls in arbitrary order. */
static void build_to(idx_t n)
{
    while (seq_len <= (size_t)n) {
        if (seq_len == 0) {            /* a(0) = 0 */
            ensure_seq_cap(1);
            seq[0] = (seq_t)0;
            ensure_used_cap(1);
            used[0] = 1;
            seq_len++;
            continue;
        }
        if (seq_len == 1) {            /* a(1) = 1 */
            ensure_seq_cap(2);
            seq[1] = (seq_t)1;
            ensure_used_cap(2);
            used[1] = 1;
            seq_len++;
            continue;
        }

        /* For n >= 2: find the smallest unused integer that keeps
         * the two relevant sums non‑prime. */
        seq_t val;
        size_t candidate = 0;
        while (1) {
            if (candidate >= used_cap)
                ensure_used_cap(candidate + 1);
            if (!used[candidate]) {
                val = (seq_t)candidate;
                /* test sums with the two previous terms */
                if (!is_prime_big(seq[seq_len - 1] + val) &&
                    !is_prime_big(seq[seq_len - 2] + val)) {
                    /* All checks passed – accept this value. */
                    if (candidate >= seq_cap)
                        ensure_seq_cap(seq_len + 1);
                    seq[seq_len] = val;
                    used[candidate] = 1;
                    break;
                }
            }
            candidate++;
        }
        seq_len++;
    }
}

/* Public function required by the harness. */
seq_t a(idx_t n)
{
    build_to(n);
    return seq[n];
}
