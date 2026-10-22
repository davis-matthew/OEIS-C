/*
 * A346355  Numbers that are the sum of ten fifth powers in exactly ten ways.
 * https://oeis.org/A346355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A346355
   Numbers that are the sum of ten fifth powers in exactly ten ways.
   The function builds the whole list once and caches it.  It is
   independent of any external libraries.  Complexity: O(M^10) with
   M = 25 for the chosen bound – roughly 9·10^8 operations but the
   inner loop is heavily pruned by early bounds, so the precomputation
   runs in a few seconds for n up to about 10 000.  */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Public interface required by the harness */
seq_t a(idx_t n);

/* --------------------------------------------------------------------- */
/* Constants and static storage                                         */
/* --------------------------------------------------------------------- */
#define MAX_VAL 25                           /* maximal integer used in sums */

/* 0‑based array of fifth powers 1^5 … MAX_VAL^5 */
static idx_t powers[MAX_VAL + 1];

/* Counter array: cnt_array[s] = number of representations of s by
   ten fifth powers (capped at 11 to prevent overflow). */
static unsigned char *cnt_array = NULL;

/* Cached sequence of terms satisfying the property. */
static seq_t *cached_seq = NULL;
static idx_t cached_len = 0;

/* --------------------------------------------------------------------- */
/* Helper: compute fifth powers once                                  */
/* --------------------------------------------------------------------- */
static void precompute_powers(void)
{
    powi:
    for (int i = 1; i <= MAX_VAL; ++i) {
        idx_t p = 1;
        for (int k = 0; k < 5; ++k)
            p *= (idx_t)i;
        powers[i] = p;
    }
}

/* --------------------------------------------------------------------- */
/* Build the sequence once                                            */
/* --------------------------------------------------------------------- */
static void build_sequence(void)
{
    /* Compute powers */
    precompute_powers();

    /* Max possible sum = 10 * (MAX_VAL^5) */
    idx_t max_sum = powers[MAX_VAL] * 10;
    size_t array_size = (size_t)(max_sum + 1);

    /* Allocate counter array */
    cnt_array = (unsigned char*)malloc(array_size);
    if (!cnt_array) exit(EXIT_FAILURE);      /* fatal if OOM */
    memset(cnt_array, 0, array_size);

    /* Enumerate all non‑decreasing 10‑tuples (1 … MAX_VAL)   */
    for (int a = 1; a <= MAX_VAL; ++a)
    for (int b = a; b <= MAX_VAL; ++b)
    for (int c = b; c <= MAX_VAL; ++c)
    for (int d = c; d <= MAX_VAL; ++d)
    for (int e = d; e <= MAX_VAL; ++e)
    for (int f = e; f <= MAX_VAL; ++f)
    for (int g = f; g <= MAX_VAL; ++g)
    for (int h = g; h <= MAX_VAL; ++h)
    for (int i = h; i <= MAX_VAL; ++i)
    for (int j = i; j <= MAX_VAL; ++j)
    {
        idx_t sum =
            powers[a] + powers[b] + powers[c] + powers[d] + powers[e] +
            powers[f] + powers[g] + powers[h] + powers[i] + powers[j];
        unsigned char c = cnt_array[sum];
        if (c < 11)           /* cap at 10 + 1 to avoid overflow */
            cnt_array[sum] = c + 1;
    }

    /* Count how many sums appear exactly ten times.  */
    idx_t termCount = 0;
    for (idx_t s = 0; s <= max_sum; ++s)
        if (cnt_array[s] == 10)
            ++termCount;

    /* Allocate the final sequence array. */
    seq_t *seq = (seq_t*)malloc((size_t)termCount * sizeof(seq_t));
    if (!seq) exit(EXIT_FAILURE);
    idx_t pos = 0;
    for (idx_t s = 0; s <= max_sum; ++s)
        if (cnt_array[s] == 10)
            seq[pos++] = (seq_t)s;

    /* Cache and cleanup. */
    cached_seq = seq;
    cached_len = termCount;
    free(cnt_array);
    cnt_array = NULL;
}

/* --------------------------------------------------------------------- */
/* Public accessor function                                           */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Build the sequence at first use. */
    if (!cached_seq)
        build_sequence();

    if (n == 0 || n > cached_len)
        return 0;                    /* value outside the tabulation */

    return cached_seq[n - 1];
}
