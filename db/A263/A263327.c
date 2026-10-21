/*
 * A263327  A permutation of {0, 1, ..., 1023} corresponding to
 *          lexicographical ordering A262557 of numbers with decreasing digits
 *          A009995.
 * https://oeis.org/A263327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = lexicographic rank of the n‑th decreasing‑digit number
 *            (numerically sorted). The function returns the value of a(n) for
 *            0 ≤ n < 1024; for larger n it returns 0
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 2, u64 n <= 1023, u128 n <= 1023,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation precomputes the permutation of {0,…,1023}. */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VAL 1024                      /* 0 … 1023 */
#define STR_BUF_SIZE 32

/* Storage for the precomputation */
static seq_t values[MAX_VAL];
static char  strbuf[MAX_VAL][STR_BUF_SIZE];
static int   num_idx[MAX_VAL];     /* indices sorted by numeric value   */
static int   lex_idx[MAX_VAL];     /* indices sorted lexicographically  */
static int   numeric_rank[MAX_VAL];/* numeric rank of each original index*/
static int   map_numeric_to_lex[MAX_VAL];
static bool  initialized = false;

/* Helper: compare indices by the numeric value of the element */
static int cmp_num(const void *a, const void *b)
{
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    if (values[ia] < values[ib]) return -1;
    if (values[ia] > values[ib]) return 1;
    return 0;
}

/* Helper: compare indices by the string representation */
static int cmp_lex(const void *a, const void *b)
{
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    return strcmp(strbuf[ia], strbuf[ib]);
}

/* Pre‑compute everything once */
static void init(void)
{
    if (initialized) return;

    /* Build the values and their string forms */
    for (int mask = 0, i = 0; mask < MAX_VAL; ++mask, ++i) {
        seq_t val = 0;
        for (int d = 9; d >= 0; --d) {
            if (mask & (1u << d)) {
                val = val * 10 + (seq_t)d;
            }
        }
        values[i] = val;

        /* Convert to string for lexicographic sort */
        int len = snprintf(strbuf[i], sizeof(strbuf[i]), "%llu",
                           (unsigned long long)val);
        if (len < 0) len = 0;
        if (len >= (int)sizeof(strbuf[i]))
            strbuf[i][sizeof(strbuf[i]) - 1] = '\0';
    }

    /* Initialise index arrays */
    for (int i = 0; i < MAX_VAL; ++i) {
        num_idx[i] = i;
        lex_idx[i] = i;
    }

    /* Sort indices by numeric value */
    qsort(num_idx, MAX_VAL, sizeof(int), cmp_num);

    /* Sort indices by lexicographic string */
    qsort(lex_idx, MAX_VAL, sizeof(int), cmp_lex);

    /* Record numeric rank of each original index */
    for (int pos = 0; pos < MAX_VAL; ++pos) {
        numeric_rank[num_idx[pos]] = pos;
    }

    /* Map numeric rank to lexicographic rank */
    for (int lex_pos = 0; lex_pos < MAX_VAL; ++lex_pos) {
        int orig = lex_idx[lex_pos];
        int num_rank = numeric_rank[orig];
        map_numeric_to_lex[num_rank] = lex_pos;
    }

    initialized = true;
}

/* Public function required by the problem statement */
seq_t a(idx_t n)
{
    if (n >= MAX_VAL) return (seq_t)0;   /* undefined for n ≥ 1024 */
    init();
    return (seq_t)map_numeric_to_lex[(size_t)n];
}
