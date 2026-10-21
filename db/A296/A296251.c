/*
 * A296251  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1)^2, where a(0) = 1, a(1) = 2, b(0) = 3, b(1) = 4, and (a(n))
 *          and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A296251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n-1)^2, * with a(0)=1, a(1)=2,
 *            b(0)=3, b(1)=4; sequences a and b are complementary increasing
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 177,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Ensure that dynamic arrays of seq_t keep the requested capacity. */
static void ensure_seq_capacity(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t new_cap = (*cap) ? (*cap) * 2 : 2;
    if (new_cap < need) new_cap = need;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* Ensure that the used‑flag array covers at least 'need' indices. */
static void ensure_used_capacity(size_t *cap, unsigned char **used, size_t need)
{
    if (*cap >= need) return;
    size_t new_cap = (*cap) ? (*cap) * 2 : 1024;
    if (new_cap < need) new_cap = need;
    unsigned char *tmp = (unsigned char *)realloc(*used, new_cap);
    if (!tmp) exit(1); /* out of memory */
    memset(tmp + *cap, 0, new_cap - *cap);
    *used = tmp;
    *cap  = new_cap;
}

/* Complementary sequences a(n) and b(n). */
seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *a_seq = NULL;
    static seq_t *b_seq = NULL;
    static size_t a_cap = 0, b_cap = 0;
    static idx_t computed_max = 0;            /* last computed index (inclusive) */
    static unsigned char *used = NULL;
    static size_t used_cap = 0;
    static idx_t next_candidate = 5;          /* smallest unused integer candidate */

    /* initialise on first call */
    if (!a_seq) {
        /* a sequence: a(0)=1, a(1)=2 */
        a_cap  = 2;
        a_seq  = (seq_t *)malloc(a_cap * sizeof(seq_t));
        a_seq[0] = (seq_t)1;
        a_seq[1] = (seq_t)2;
        computed_max = 1;

        /* b sequence: b(0)=3, b(1)=4 */
        b_cap = 2;
        b_seq = (seq_t *)malloc(b_cap * sizeof(seq_t));
        b_seq[0] = (seq_t)3;
        b_seq[1] = (seq_t)4;

        /* used flag array */
        used_cap = 1024;
        used = (unsigned char *)malloc(used_cap);
        memset(used, 0, used_cap);
        /* mark 1..4 as used */
        for (size_t i = 1; i <= 4; ++i) used[i] = 1;
        next_candidate = 5;
    }

    /* quick return for already computed indices */
    if (n <= computed_max) return a_seq[(size_t)n];

    /* compute forward until reach n */
    for (idx_t idx = computed_max + 1; idx <= n; ++idx) {
        /* compute a[idx] = a[idx-1] + a[idx-2] + b[idx-1]^2 */
        seq_t a1 = a_seq[(size_t)(idx - 1)];
        seq_t a2 = a_seq[(size_t)(idx - 2)];
        seq_t b1 = b_seq[(size_t)(idx - 1)];
        seq_t a_new = a1 + a2 + (b1 * b1);

        ensure_seq_capacity(&a_seq, &a_cap, (size_t)(idx + 1));
        a_seq[(size_t)idx] = a_new;

        /* mark a_new as used if it fits in the used array */
        seq_t used_limit = (seq_t)(used_cap - 1);
        if (a_new <= used_limit) used[(size_t)a_new] = 1;

        /* make sure the used array can accommodate the next candidate */
        ensure_used_capacity(&used_cap, &used, (size_t)(next_candidate + 1));

        /* find the next unused integer for b[idx] */
        while (used[(size_t)next_candidate]) ++next_candidate;
        seq_t b_new = (seq_t)next_candidate;

        ensure_seq_capacity(&b_seq, &b_cap, (size_t)(idx + 1));
        b_seq[(size_t)idx] = b_new;

        /* mark this number as used and move to the next candidate */
        used[(size_t)next_candidate] = 1;
        ++next_candidate;
    }

    computed_max = n;
    return a_seq[(size_t)n];
}
