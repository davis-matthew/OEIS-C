/*
 * A374966  a(n) = 0 if n has appeared in the sequence before, otherwise a(n)
 *          = a(n-1) + n. Start with a(0) = 0.
 * https://oeis.org/A374966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(0) = 0; a(n) = 0 if n has appeared as a previous term, a(n) =
 *            a(n-1) + n otherwise. (OEIS A374966)
 * Range:     u8 n <= 39, u16 n <= 2116, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* External types supplied by the harness */
extern seq_t a(idx_t n);         /* forward declaration used by compiler */

/* Dynamic arrays for memoisation and membership tracking */
static seq_t *memo = NULL;       /* memoised a(k) values */
static size_t memo_cap = 0;      /* capacity of memo array */
static size_t memo_len = 0;      /* number of computed entries (>= memo_cap) */

static seq_t *seen = NULL;       /* list of values that have appeared as a(k) */
static size_t seen_cap = 0;      /* capacity of seen array */
static size_t seen_cnt = 0;      /* number of recorded values */

/* Ensure memo array has room for at least 'cap' elements. */
static void grow_memo(size_t cap)
{
    if (cap <= memo_cap)
        return;
    size_t new_cap = memo_cap ? memo_cap : 1;
    while (new_cap < cap)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                /* unrecoverable failure */
    memo = tmp;
    memo_cap = new_cap;
}

/* Ensure seen array has room for at least 'cap' elements. */
static void grow_seen(size_t cap)
{
    if (cap <= seen_cap)
        return;
    size_t new_cap = seen_cap ? seen_cap : 1;
    while (new_cap < cap)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seen, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    seen = tmp;
    seen_cap = new_cap;
}

/* Linear search to test whether 'v' is already present. */
static bool seen_before(idx_t v)
{
    seq_t val = (seq_t)v;
    for (size_t i = 0; i < seen_cnt; ++i)
        if (seen[i] == val)
            return true;
    return false;
}

/* Append a value to the seen array. */
static void add_seen(seq_t v)
{
    grow_seen(seen_cnt + 1);
    seen[seen_cnt++] = v;
}

/* Compute a(k) for all 0 ≤ k ≤ n if not already done. */
static void extend_to(idx_t n)
{
    /* If we are already past the requested index, nothing to do. */
    if ((size_t)n < memo_len)
        return;

    /* Allocate memo array large enough. */
    grow_memo((size_t)n + 1);

    /* Process terms from the next uncomputed index up to n. */
    for (idx_t k = memo_len; k <= n; ++k) {
        if (k == 0) {
            memo[0] = (seq_t)0;
            add_seen(0);              /* 0 appears as a(0) */
        } else if (seen_before(k)) {
            memo[k] = (seq_t)0;
            add_seen(0);              /* zero for this repeated index */
        } else {
            seq_t val = memo[k - 1] + (seq_t)k;
            memo[k] = val;
            add_seen(val);            /* record the new term */
        }
    }
    memo_len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    extend_to(n);
    return memo[(size_t)n];
}
