/*
 * A296294  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          n*b(n), where a(0) = 1, a(1) = 3, b(0) = 2, b(1) = 4, b(2) = 5,
 *          and (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A296294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + n*b(n) with a(0)=1, a(1)=3, b(0)=2,
 *            b(1)=4, b(2)=5 (a(n)) and (b(n)) are increasing complementary
 *            sequences
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 177,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *a_seq = NULL;
static seq_t *b_seq = NULL;
static seq_t *used   = NULL;
static size_t a_cap  = 0, b_cap = 0, used_cap = 0;
static size_t a_len  = 0, b_len = 0, used_len = 0;
static bool initialized = false;

/* ensure a capacity for an array of seq_t elements */
static void ensure_cap(size_t *cap, seq_t **arr, size_t needed)
{
    if (*cap >= needed) return;
    size_t newcap = (*cap > 0) ? (*cap * 2) : 8;
    while (newcap < needed) newcap <<= 1;
    *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    *cap = newcap;
}

/* linear search for a value in the used array */
static bool is_used(seq_t x)
{
    for (size_t i = 0; i < used_len; ++i)
        if (used[i] == x) return true;
    return false;
}

seq_t a(idx_t n)
{
    /* initialize on first call */
    if (!initialized) {
        initialized = true;
        a_cap  = 0;
        b_cap  = 0;
        used_cap = 0;
        a_len = b_len = used_len = 0;
        a_seq = NULL;
        b_seq = NULL;
        used   = NULL;
    }

    /* return cached value if available */
    if ((idx_t)a_len > n) return a_seq[n];

    /* extend storage to accommodate index n */
    ensure_cap(&a_cap, &a_seq, (size_t)n + 1);
    ensure_cap(&b_cap, &b_seq, (size_t)n + 1);
    ensure_cap(&used_cap, &used, 2 * ((size_t)n + 1));

    /* compute missing terms up to n */
    for (idx_t curr = (idx_t)a_len; curr <= n; ++curr) {
        if (curr == 0) {
            a_seq[0] = (seq_t)1;
            b_seq[0] = (seq_t)2;
            used[used_len++] = a_seq[0];
            used[used_len++] = b_seq[0];
            continue;
        }
        if (curr == 1) {
            a_seq[1] = (seq_t)3;
            b_seq[1] = (seq_t)4;
            used[used_len++] = a_seq[1];
            used[used_len++] = b_seq[1];
            continue;
        }
        /* previous values */
        seq_t a_prev  = a_seq[curr - 1];
        seq_t a_prev2 = a_seq[curr - 2];
        seq_t prev_b  = b_seq[curr - 1];

        /* find smallest unused b > prev_b such that a is also unused */
        seq_t cand = prev_b + (seq_t)1;
        while (true) {
            /* skip if cand already used */
            if (is_used(cand)) { ++cand; continue; }
            seq_t a_candidate = a_prev + a_prev2 + (seq_t)curr * cand;
            if (is_used(a_candidate)) { ++cand; continue; }
            /* good pair */
            b_seq[curr] = cand;
            a_seq[curr] = a_candidate;
            used[used_len++] = cand;
            used[used_len++] = a_candidate;
            break;
        }
    }

    a_len = (size_t)n + 1;
    b_len = (size_t)n + 1;
    return a_seq[n];
}
