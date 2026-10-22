/*
 * A367810  Lexicographically earliest sequence of distinct positive terms not
 *          ending in 0 such that the Levenshtein distance (Ld) between a(n)
 *          and a(n+1) is equal to the last digit of a(n).
 * https://oeis.org/A367810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes: a(n) = lexicographically earliest distinct positive 
   terms not ending in 0 such that Ld(a(n), a(n+1)) == last digit of a(n).
   Sequence indexes from 1.
*/

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_to_str.h"

/* Levenshtein distance between the decimal digit strings of two numbers.
   The digits are taken most‑significant first, as produced by seq_to_str. */
static int levenshtein_seq(seq_t a, seq_t b)
{
    char bufA[4096], bufB[4096];
    size_t lenA, lenB;
    seq_to_str(a, bufA, &lenA);
    seq_to_str(b, bufB, &lenB);

    int *rowPrev = (int *)calloc(lenB + 1, sizeof(int));
    int *rowCurr = (int *)calloc(lenB + 1, sizeof(int));
    if (!rowPrev || !rowCurr) { exit(1); }

    for (size_t j = 0; j <= lenB; ++j) rowPrev[j] = (int)j;

    for (size_t i = 1; i <= lenA; ++i) {
        rowCurr[0] = (int)i;
        char chA = bufA[i - 1];
        for (size_t j = 1; j <= lenB; ++j) {
            char chB = bufB[j - 1];
            int cost = (chA == chB) ? 0 : 1;
            int del = rowPrev[j] + 1;
            int ins = rowCurr[j - 1] + 1;
            int sub = rowPrev[j - 1] + cost;
            int best = del < ins ? del : ins;
            if (sub < best) best = sub;
            rowCurr[j] = best;
        }
        int *tmp = rowPrev;
        rowPrev = rowCurr;
        rowCurr = tmp;
    }
    int dist = rowPrev[lenB];
    free(rowPrev);
    free(rowCurr);
    return dist;
}

/* Ensure a dynamic array has at least 'need' elements. */
static void grow_array(void **arr, size_t *cap, size_t need, size_t elem_size)
{
    if (need <= *cap) return;
    size_t new_cap = (*cap == 0) ? 8 : *cap << 1;
    while (new_cap < need) new_cap <<= 1;
    void *tmp = realloc(*arr, new_cap * elem_size);
    if (!tmp) exit(1);
    *arr = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* unused pre‑offset */

    /* static storage for the sequence and its used values */
    static seq_t *seq = NULL;
    static size_t seq_len = 0, seq_cap = 0;
    static seq_t *used = NULL;
    static size_t used_len = 0, used_cap = 0;

    /* at least capacity for index n */
    grow_array((void **)&seq, &seq_cap, n, sizeof(seq_t));

    /* initialise the first element if the table is empty */
    if (seq_len == 0) {
        seq[0] = (seq_t)1;
        seq_len = 1;
        grow_array((void **)&used, &used_cap, 1, sizeof(seq_t));
        used[0] = seq[0];
        used_len = 1;
    }

    /* extend the sequence up to n */
    while (seq_len < n) {
        seq_t cur = seq[seq_len - 1];
        uint8_t last_digit = (uint8_t)(cur % (seq_t)10);

        seq_t candidate = 0;
        seq_t cand = (seq_t)1;
        for (;; ++cand) {
            if ((cand % (seq_t)10) == 0) continue;          /* skip trailing 0 */

            /* check uniqueness */
            bool dup = false;
            for (size_t k = 0; k < used_len; ++k) {
                if (used[k] == cand) { dup = true; break; }
            }
            if (dup) continue;

            if (levenshtein_seq(cur, cand) == last_digit) {
                candidate = cand;
                break;
            }
        }

        seq[seq_len++] = candidate;

        grow_array((void **)&used, &used_cap, used_len + 1, sizeof(seq_t));
        used[used_len++] = candidate;
    }

    return seq[n - 1];
}
