/*
 * A317015  a(n) = n for n < 2, a(n) = a(freq(a(n-1),n)) + a(freq(a(n-2),n))
 *          for n >= 2, where freq(i, j) is the number of times i appears in
 *          the first j terms.
 * https://oeis.org/A317015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8076, u16 n <= 8076, u32 n <= 8076, u64 n <= 8076,
 *            u128 n <= 8076, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A317015
 * a(n) = n            for n < 2
 * a(n) = a(freq(a(n-1), n)) + a(freq(a(n-2), n))   for n ≥ 2
 * where freq(i, j) counts the occurrences of i among a(0) … a(j‑1).
 *
 * The implementation memoises all computed terms in a dynamic array and
 * uses linear scans to obtain the required frequencies.  Since freq(i,j)
 * never exceeds j, the referenced indices are already known when needed.
 */

#include <stdlib.h>          /* malloc, realloc, abort */

static seq_t *seq = NULL;       /* stored a(0)…a(k)       */
static size_t seq_len = 0;      /* number of stored terms */
static size_t seq_cap = 0;      /* allocated capacity */

/* Ensure seq has room for at least 'need' elements. */
static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 1;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    seq = tmp;
    seq_cap = new_cap;
}

/* Initialise the first two terms a(0)=0, a(1)=1. */
static void init_sequence(void)
{
    if (seq_len != 0)
        return;
    ensure_seq_capacity(2);
    seq[0] = (seq_t)0;
    seq[1] = (seq_t)1;
    seq_len = 2;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    /* Base cases a(0)=0, a(1)=1. */
    if (n < (idx_t)2)
        return (seq_t)n;

    init_sequence();

    size_t target = (size_t)n;
    if (target < seq_len)
        return seq[target];

    /* Extend the sequence up to the requested index. */
    ensure_seq_capacity(target + 1);

    for (size_t i = seq_len; i <= target; ++i)
    {
        seq_t prev1 = seq[i - 1];
        seq_t prev2 = seq[i - 2];
        size_t c1 = 0, c2 = 0;

        /* Count occurrences of prev1 and prev2 in a[0 .. i-1]. */
        for (size_t j = 0; j < i; ++j)
        {
            if (seq[j] == prev1)
                ++c1;
            if (seq[j] == prev2)
                ++c2;
        }

        /* Recurrence: a(i) = a(c1) + a(c2). */
        seq[i] = seq[c1] + seq[c2];
    }

    seq_len = target + 1;
    return seq[target];
}
