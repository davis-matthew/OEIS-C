/*
 * A387170  Self-inventory sequence modulo 10: start with 0; repeatedly append
 *          the counts of digits 0..9 seen so far, taken mod 10.
 * https://oeis.org/A387170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; for n ≥ 1: a(n) is the current count modulo 10 of
 *            digit ((n-1) mod 10) in the sequence a(0)…a(n-1). The counts
 *            themselves are maintained modulo 10, and each new term is
 *            appended as the next digit of the sequence
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>           /* malloc, exit */
#include <stddef.h>           /* size_t */
#include "../utils/seq_alloc.h"      /* seq_alloc(size_t) */
#include "../utils/seq_realloc.h"    /* seq_realloc(seq_t*, size_t, size_t) */

static seq_t *seq = NULL;          /* cached sequence values */
static size_t seq_cap = 0;         /* allocated capacity of seq */
static size_t seq_len = 0;         /* number of terms currently stored */
static unsigned char cnt[10] = {0, /* counts of digits 0..9 modulo 10 */ };

static void init_seq(void)
{
    if (seq != NULL)
        return;                         /* already initialized */

    /* initial allocation – 128 is arbitrary, will grow as needed */
    seq_cap = 128;
    seq = seq_alloc(seq_cap);
    if (seq == NULL)
        exit(EXIT_FAILURE);

    /* a(0) = 0; only one digit 0 has been seen so far */
    seq[0]   = (seq_t)0;
    seq_len  = 1;
    cnt[0]   = 1;                /* 1 zero seen so far */
}

/* grow the cached array to at least 'need' elements */
static void grow_seq(size_t need)
{
    size_t new_cap = seq_cap;
    while (new_cap < need)
        new_cap <<= 1;            /* double until large enough */

    seq_t *tmp = seq_realloc(seq, seq_cap, new_cap);
    if (tmp == NULL)
        exit(EXIT_FAILURE);

    seq      = tmp;
    seq_cap  = new_cap;
}

seq_t a(idx_t n)
{
    init_seq();                   /* ensure sequence is ready */

    if ((size_t)n < seq_len)      /* already computed */
        return seq[(size_t)n];

    /* enlarge cache if necessary */
    grow_seq((size_t)n + 1);

    /* compute terms up to n (inclusive) */
    for (idx_t i = (idx_t)seq_len; i <= n; ++i)
    {
        unsigned int d = (unsigned int)((i - 1) % 10);   /* digit to count */
        seq_t term = (seq_t)cnt[d];                      /* output digit */

        seq[(size_t)i] = term;                          /* store result */

        /* update the count of the digit we just appended */
        unsigned char *p = &cnt[(size_t)term];
        ++(*p);
        if (*p == 10)
            *p = 0;
    }

    seq_len = (size_t)n + 1;
    return seq[(size_t)n];
}
