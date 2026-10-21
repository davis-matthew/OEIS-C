/*
 * A297833  Solution of the complementary equation a(n) = a(1)*b(n-1) -
 *          a(0)*b(n-2) + 2*n - 3, where a(0) = 1, a(1) = 2, b(0) = 3, b(1) =
 *          4, and (b(n)) is the increasing sequence of positive integers not
 *          in (a(n)). See Comments.
 * https://oeis.org/A297833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A297833
 *  Recurrence:  a(n) = a(1)*b(n-1) - a(0)*b(n-2) + 2*n - 3
 *  with a(0) = 1, a(1) = 2, b(0) = 3, b(1) = 4,
 *  and b(n) is the increasing sequence of positive integers not in a().
 */

/* required headers – works in C and C++ */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Static tables for the two complementary sequences                */
/* ------------------------------------------------------------------ */
static seq_t *a_arr = NULL;
static seq_t *b_arr = NULL;
static size_t a_cap = 0, b_cap = 0;
static size_t a_len = 0, b_len = 0;

/*  Bitmap: 1 means the integer is already used by a() or b()         */
static unsigned char *used = NULL;
static size_t used_cap = 0;

/*  Next integer to consider for the b sequence                      */
static idx_t next_b_candidate = 1;

/* ------------------------------------------------------------------ */
/*  Helper: enlarge the used bitmap so that index 'n' is valid        */
/* ------------------------------------------------------------------ */
static void ensure_used_cap(idx_t n)
{
    if ((size_t)n < used_cap)
        return;
    size_t new_cap = used_cap ? used_cap * 2 : 64;
    while (new_cap <= (size_t)n)
        new_cap *= 2;
    unsigned char *tmp = (unsigned char *)realloc(used, new_cap);
    if (!tmp)
        abort();
    memset(tmp + used_cap, 0, new_cap - used_cap);
    used = tmp;
    used_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Generate b(k) for all k <= idx                                   */
/* ------------------------------------------------------------------ */
static void generate_b_to(idx_t idx)
{
    while (b_len <= (size_t)idx) {
        /* skip numbers already used by a or earlier b */
        ensure_used_cap(next_b_candidate);
        while (used[(size_t)next_b_candidate])
            ++next_b_candidate;

        seq_t val = (seq_t)next_b_candidate;

        if (b_len >= b_cap) {
            b_cap = b_cap ? b_cap * 2 : 8;
            seq_t *tmp = (seq_t *)realloc(b_arr, b_cap * sizeof(seq_t));
            if (!tmp)
                abort();
            b_arr = tmp;
        }
        b_arr[b_len++] = val;

        /* mark the value as used if it fits into the index type */
        if (val <= (seq_t)(~(idx_t)0)) {
            idx_t v = (idx_t)val;
            ensure_used_cap(v);
            used[(size_t)v] = 1;
        }
        ++next_b_candidate;
    }
}

/* ------------------------------------------------------------------ */
/*  Initialise the tables with the first two terms                    */
/* ------------------------------------------------------------------ */
static void initialized(void)
{
    if (a_len != 0)
        return;

    a_cap = 8; a_arr = (seq_t *)malloc(a_cap * sizeof(seq_t));
    b_cap = 8; b_arr = (seq_t *)malloc(b_cap * sizeof(seq_t));

    a_arr[0] = (seq_t)1;
    a_arr[1] = (seq_t)2;
    b_arr[0] = (seq_t)3;
    b_arr[1] = (seq_t)4;
    a_len = b_len = 2;

    used_cap = 64;
    used = (unsigned char *)calloc(used_cap, 1);
    used[1] = used[2] = used[3] = used[4] = 1;
}

/* ------------------------------------------------------------------ */
/*  Ensure all a(0 .. n) are known                                   */
/* ------------------------------------------------------------------ */
static void ensure_a_to(idx_t n)
{
    initialized();
    while (a_len <= (size_t)n) {
        /* ensure the required b terms exist */
        generate_b_to((idx_t)a_len - 1);
        generate_b_to((idx_t)a_len - 2);

        seq_t val = a_arr[1] * b_arr[(size_t)a_len - 1]
                    - a_arr[0] * b_arr[(size_t)a_len - 2]
                    + ((seq_t)2 * (seq_t)(a_len))
                    - ((seq_t)3);

        if (a_len >= a_cap) {
            a_cap = a_cap ? a_cap * 2 : 8;
            seq_t *tmp = (seq_t *)realloc(a_arr, a_cap * sizeof(seq_t));
            if (!tmp)
                abort();
            a_arr = tmp;
        }
        a_arr[a_len++] = val;

        if (val <= (seq_t)(~(idx_t)0)) {
            idx_t v = (idx_t)val;
            ensure_used_cap(v);
            used[(size_t)v] = 1;
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Public API – required function                                    */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < a_len)
        return a_arr[n];
    ensure_a_to(n);
    return a_arr[n];
}
