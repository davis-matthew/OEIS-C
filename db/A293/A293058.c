/*
 * A293058  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-2) + 3, where a(0) = 1, a(1) = 3, b(0) = 2, b(1) = 4.
 * https://oeis.org/A293058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n-2) + 3 Initial values: a(0)=1,
 *            a(1)=3, b(0)=2, b(1)=4
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ---------- dynamic array helpers ---------- */

static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t newcap = *cap ? *cap * 2 : 8;
        while (newcap < need) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        if (!tmp) abort();
        *arr = tmp;
        *cap = newcap;
    }
}

static void grow_bool(bool **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t newcap = *cap ? *cap * 2 : 8;
        while (newcap < need) newcap <<= 1;
        bool *tmp = (bool *)realloc(*arr, newcap * sizeof(bool));
        if (!tmp) abort();
        /* initialise new slots to false */
        for (size_t i = *cap; i < newcap; ++i) tmp[i] = false;
        *arr = tmp;
        *cap = newcap;
    }
}

/* ---------- global state ---------- */

static seq_t *a_seq = NULL;
static seq_t *b_seq = NULL;
static size_t cap_a = 0, cap_b = 0;
static size_t len = 0;          /* number of terms computed so far */

static bool *used = NULL;
static size_t used_cap = 0;
static idx_t next_candidate = 0;  /* next integer to test for b() */

/* ---------- initialization ---------- */

static void init(void)
{
    /* allocate sequences with some capacity */
    cap_a = cap_b = 8;
    a_seq = (seq_t *)malloc(cap_a * sizeof(seq_t));
    b_seq = (seq_t *)malloc(cap_b * sizeof(seq_t));
    if (!a_seq || !b_seq) abort();

    /* set initial values */
    a_seq[0] = (seq_t)1;
    a_seq[1] = (seq_t)3;
    b_seq[0] = (seq_t)2;
    b_seq[1] = (seq_t)4;
    len = 2;                  /* indices 0 and 1 are available */

    /* used table for the small numbers that appear early */
    used_cap = 64;
    used = (bool *)calloc(used_cap, sizeof(bool));
    if (!used) abort();

    /* mark the initial numbers as used */
    used[1] = used[2] = used[3] = used[4] = true;
    next_candidate = 5;        /* next integer to test for b() */
}

/* ---------- main function ---------- */

seq_t a(idx_t n)
{
    /* initialise on first use */
    if (a_seq == NULL) init();

    /* extend the sequences up to index n */
    if ((idx_t)len <= n) {
        size_t target = (size_t)n;
        for (size_t i = len; i <= target; ++i) {
            /* ensure enough space */
            grow_seq(&a_seq, &cap_a, i + 1);
            grow_seq(&b_seq, &cap_b, i + 1);

            /* compute a(i) */
            seq_t ai = a_seq[i-1] + a_seq[i-2] + b_seq[i-2] + (seq_t)3;
            a_seq[i] = ai;

            /* compute b(i) : smallest unused positive integer (≥1) */
            idx_t bi = next_candidate;
            for (;;) {
                size_t ui = (size_t)bi;
                if (ui >= used_cap) grow_bool(&used, &used_cap, ui + 1);
                if (!used[ui]) break;
                ++bi;
            }
            b_seq[i] = bi;
            used[(size_t)bi] = true;            /* mark b(i) as used */
            next_candidate = bi + 1;            /* next search starts here */

            /* mark a(i) as used if it falls inside the current table */
            size_t ui = (size_t)ai;
            if (ui < used_cap) used[ui] = true;
            /* otherwise we omit it; it will never affect future b() searches,
               because b() grows only linearly while a() grows exponentially. */

            ++len;   /* one more term computed */
        }
    }

    return a_seq[(size_t)n];
}
