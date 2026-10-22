/*
 * A340446  Lexicographically earliest sequence of distinct nonnegative
 *          integers such that for any n >= 0, a(n) XOR a(n+1) is a positive
 *          cube as small as possible (where XOR denotes the bitwise XOR
 *          operator).
 * https://oeis.org/A340446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lexicographically earliest permutation of nonnegative
 *            integers such that a(n) XOR a(n+1) is a positive cube as small
 *            as possible. The sequence is computed by greedy construction
 * Range:     u8 n <= 63, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static arrays holding the computed sequence and the set of values used.
   Both grow as needed.  All indices and sizes are kept in size_t as they
   are never larger than the number of computed terms (≤ idx_t). */
static seq_t *seq = NULL;
static size_t seq_len = 0, seq_cap = 0;

static seq_t *used = NULL;
static size_t used_len = 0, used_cap = 0;

/* Ensure capacity for the sequence array.  Called with the minimum
   required capacity (≥ needed).  Capacity grows exponentially. */
static void ensure_seq(size_t needed)
{
    if (seq_cap < needed) {
        size_t newcap = (seq_cap > 0) ? seq_cap * 2 : 64;
        while (newcap < needed) newcap <<= 1;
        seq = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
        seq_cap = newcap;
    }
}

/* Ensure capacity for the used values array.  */
static void ensure_used(size_t needed)
{
    if (used_cap < needed) {
        size_t newcap = (used_cap > 0) ? used_cap * 2 : 64;
        while (newcap < needed) newcap <<= 1;
        used = (seq_t *)realloc(used, newcap * sizeof(seq_t));
        used_cap = newcap;
    }
}

/* Check whether value x has already appeared in the sequence.  */
static int used_contains(seq_t x)
{
    size_t i;
    for (i = 0; i < used_len; ++i) {
        if (used[i] == x) return 1;
    }
    return 0;
}

/* Append value x to the used set (stored nondistinctly).  */
static void used_append(seq_t x)
{
    ensure_used(used_len + 1);
    used[used_len++] = x;
}

seq_t a(idx_t n)
{
    /* Compute the sequence up to index n (if not already done). */
    if (seq_len == 0) {
        /* Initialise the sequence with a(0) = 0.  */
        ensure_seq(1);
        seq[0] = (seq_t)0;
        seq_len = 1;
        ensure_used(1);
        used[0] = (seq_t)0;
        used_len = 1;
    }

    /* Expand to include index n.  */
    if (seq_len <= (size_t)n) {
        size_t i;
        for (i = seq_len; i <= (size_t)n; ++i) {
            seq_t cur = seq[i - 1];  /* current term */
            seq_t nxt = 0;
            idx_t k = 1;
            for (;;) {
                /* Compute k^3 in seq_t.  */
                seq_t kk = (seq_t)k;
                seq_t cube = kk * kk * kk;
                seq_t y = cur ^ cube;          /* candidate for a(i) */
                if (!used_contains(y)) {
                    nxt = y;
                    break;
                }
                ++k;
            }
            ensure_seq(i + 1);
            seq[i] = nxt;
            used_append(nxt);
        }
        seq_len = (size_t)n + 1;
    }

    return seq[(size_t)n];
}
