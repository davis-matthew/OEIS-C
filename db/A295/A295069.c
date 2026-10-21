/*
 * A295069  Solution of the complementary equation a(n) = 2*a(n-2) - b(n-2) +
 *          n, where a(0) = 3, a(1) = 4, b(0) = 1, and (a(n)) and (b(n)) are
 *          increasing complementary sequences.
 * https://oeis.org/A295069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) - b(n-2) + n Initial values: a(0)=3, a(1)=4,
 *            b(0)=1 (a(n)) and (b(n)) are complementary increasing sequences
 * Range:     u8 n <= 13, u16 n <= 29, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   find_b
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/find_b.h"

static seq_t *a_arr      = NULL; /* holds a(n) values */
static seq_t *pref_arr   = NULL; /* pref[i] = a[i] - (i+1) */
static seq_t *b_arr      = NULL; /* holds computed b(k) values */
static idx_t  a_size     = 0;    /* number of a(n) values stored */
static idx_t  a_capacity = 0;

/* enlarge the static arrays so they can hold at least `needed` elements */
static void ensure_capacity(idx_t needed)
{
    if (a_capacity >= needed) return;
    idx_t newcap = a_capacity ? a_capacity << 1 : 1;
    while (newcap < needed) newcap <<= 1;

    a_arr    = (seq_t*)realloc(a_arr,    newcap * sizeof(seq_t));
    pref_arr = (seq_t*)realloc(pref_arr, newcap * sizeof(seq_t));
    b_arr    = (seq_t*)realloc(b_arr,    newcap * sizeof(seq_t));
    a_capacity = newcap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)4;

    ensure_capacity(n + 1);

    /* initialise first two terms if they are not yet present */
    if (a_size == 0) {
        a_arr[0] = (seq_t)3;
        pref_arr[0] = a_arr[0] - ((seq_t)1);
        b_arr[0] = (seq_t)1;
        a_size = 1;
    }
    if (a_size == 1) {
        a_arr[1] = (seq_t)4;
        pref_arr[1] = a_arr[1] - ((seq_t)2);
        a_size = 2;
    }

    /* compute a(n) up to requested index */
    while (a_size <= n) {
        idx_t i = a_size;                 /* compute a[i] */
        idx_t b_needed = i >= 2 ? i - 2 : 0;

        /* b(b_needed) is needed */
        seq_t bval = find_b(b_needed, a_arr, pref_arr, a_size - 1);
        b_arr[b_needed] = bval;

        seq_t term = (seq_t)2 * a_arr[i - 2] - bval + (seq_t)i;
        a_arr[i] = term;
        pref_arr[i] = term - ((seq_t)(i + 1));

        a_size = i + 1;   /* one more a value stored */
    }

    return a_arr[n];
}
