/*
 * A299975  Lexicographic first sequence of positive integers such that a(n) +
 *          a(n+1) has a digit 5, and no term occurs twice.
 * https://oeis.org/A299975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Lexicographic first permutation of the positive integers such
 *            that the sum of successive terms contains the digit 5, and no
 *            term repeats
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   has_digit5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/has_digit5.h"

/* Dynamically sized arrays */
static seq_t *seq   = NULL;          /* stored sequence terms, 0‑based indexes */
static idx_t  seq_cap  = 0;

static uint8_t *used  = NULL;        /* used[i] == 1 if integer i has been used */
static idx_t   used_cap = 0;

static idx_t target_len = 0;         /* length of sequence currently being built */

/* Forward declaration of recursive search */
static bool dfs(idx_t pos);

/* Ensure used can index up to idx (inclusive) */
static void ensure_used_cap(idx_t idx)
{
    if (idx < used_cap) return;
    idx_t new_cap = used_cap ? used_cap : 1;
    while (new_cap <= idx) new_cap <<= 1;
    uint8_t *tmp = (uint8_t*)realloc(used, new_cap);
    if (!tmp) abort();
    /* new part is zeroed */
    memset(tmp + used_cap, 0, new_cap - used_cap);
    used = tmp;
    used_cap = new_cap;
}

/* Ensure seq can hold at least cnt elements */
static void ensure_seq_cap(idx_t cnt)
{
    if (cnt <= seq_cap) return;
    idx_t new_cap = seq_cap ? seq_cap : 1;
    while (new_cap < cnt) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq = tmp;
    seq_cap = new_cap;
}

/* Recursive depth‑first search that builds target_len terms. */
static bool dfs(idx_t pos)
{
    if (pos >= target_len) return true;   /* finished */

    seq_t prev = seq[pos - 1];

    for (seq_t cand = 1; ; ++cand) {
        idx_t ci = (idx_t)cand;
        if (ci >= used_cap) ensure_used_cap(ci);

        if (!used[ci] && has_digit5(prev + cand)) {
            used[ci]   = 1;
            seq[pos]   = cand;
            if (dfs(pos + 1)) return true;    /* success */
            used[ci]   = 0;                    /* backtrack */
        }
    }
    return false;   /* unreachable */
}

/* Compute the sequence up to length N (N >= 1). */
static void compute_to(idx_t N)
{
    if (N < 1) return;

    /* prepare storage */
    ensure_seq_cap(N);
    ensure_used_cap(N);           /* at least N in case early values are needed */

    /* clear used array */
    memset(used, 0, used_cap);

    /* first term is fixed */
    seq[0] = (seq_t)1;
    used[1] = 1;

    target_len = N;
    if (!dfs(1)) abort();         /* a valid sequence always exists */
}

/* Public function required by the template. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safe guard for undefined offsets */

    static idx_t computed = 0;     /* longest length already computed */

    if (n > computed) {
        compute_to(n);
        computed = n;
    }
    return seq[n - 1];
}
