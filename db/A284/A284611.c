/*
 * A284611  Number of partitions of n such that the (sum of all even parts) =
 *          floor(n/2).
 * https://oeis.org/A284611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = ⎧ * 0 if floor(n/2) is odd *
 *            p_unrestricted(floor(n/2)/2) * p_distinct(floor(n/2) + (n mod
 *            2)) * ⎫ * where p_unrestricted(k) is the usual partition number
 *            of k, * and p_distinct(k) is the number of partitions of k into
 *            distinct parts * (equivalently, partitions of k into odd parts).
 *            This...
 * Range:     u8 n <= 27, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *unrestricted = NULL;   /* partitions of k (unrestricted) */
static size_t unrestricted_len = 0;  /* array length in elements */

static seq_t *distinct = NULL;       /* partitions of k into distinct parts */
static size_t distinct_len = 0;      /* array length in elements */

/* Ensure array for unrestricted partitions up to 'limit' (inclusive) */
static void ensure_unrestricted(idx_t limit)
{
    if ((size_t)(limit + 1) <= unrestricted_len)
        return;
    size_t new_len = (size_t)(limit + 1);
    seq_t *buf = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (!buf) return; /* allocation failed; let caller handle zero result */
    for (size_t i = 0; i < new_len; ++i) buf[i] = 0;
    buf[0] = 1; /* empty partition */

    for (idx_t coin = 1; coin <= limit; ++coin) {
        for (idx_t j = coin; j <= limit; ++j) {
            buf[j] += buf[j - coin];
        }
    }

    free(unrestricted);
    unrestricted = buf;
    unrestricted_len = new_len;
}

/* Ensure array for distinct-part partitions up to 'limit' (inclusive) */
static void ensure_distinct(idx_t limit)
{
    if ((size_t)(limit + 1) <= distinct_len)
        return;
    size_t new_len = (size_t)(limit + 1);
    seq_t *buf = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (!buf) return; /* allocation failed; let caller handle zero result */
    for (size_t i = 0; i < new_len; ++i) buf[i] = 0;
    buf[0] = 1; /* empty partition */

    for (idx_t coin = 1; coin <= limit; ++coin) {
        for (idx_t j = limit; j >= coin; --j) {
            buf[j] += buf[j - coin];
        }
    }

    free(distinct);
    distinct = buf;
    distinct_len = new_len;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* outside of offset range */

    idx_t t = n / 2;            /* floor(n/2) */

    /* Determine required sizes */
    idx_t s_needed = 0;
    if (t % 2 == 0) s_needed = t / 2;
    idx_t d_needed = t + (n & 1);   /* t + (n mod 2) */

    /* Compute arrays up to the needed limits */
    ensure_unrestricted(s_needed);
    ensure_distinct(d_needed);

    /* Partition of even parts */
    seq_t p_even = 0;
    if (t % 2 == 0) {
        if ((size_t)s_needed < unrestricted_len)
            p_even = unrestricted[s_needed];
    }

    /* Partition of odd parts */
    seq_t p_odd = 0;
    if ((size_t)d_needed < distinct_len)
        p_odd = distinct[d_needed];

    return p_even * p_odd;
}
