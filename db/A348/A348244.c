/*
 * A348244  Indices of 1's in A307720.
 * https://oeis.org/A348244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the n-th 1 in A307720; computed by generating
 *            A307720 greedily (lexicographically earliest, exactly k adjacent
 *            pairs with product k) and recording positions of 1's
 * Range:     u8 n <= 10, u16 n <= 66, u32 n <= 927, u64 n <= 927,
 *            u128 n <= 927, big any n
 * Validated: standard, pass, 69 terms through n = 821 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static size_t *A348244_cnt = NULL;
static size_t A348244_cnt_cap = 0;
static idx_t *A348244_ones = NULL;
static size_t A348244_ones_len = 0;
static size_t A348244_ones_cap = 0;
static idx_t A348244_pos = 0;      /* number of terms generated */
static seq_t A348244_prev;         /* last generated term */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    if (A348244_pos == 0) {
        A348244_cnt_cap = 1024;
        A348244_cnt = (size_t *)calloc(A348244_cnt_cap, sizeof(size_t));
        A348244_ones_cap = 64;
        A348244_ones = (idx_t *)malloc(A348244_ones_cap * sizeof(idx_t));
        A348244_ones[0] = 1;
        A348244_ones_len = 1;
        A348244_pos = 1;
        A348244_prev = (seq_t)1;
    }

    while (A348244_ones_len < (size_t)n) {
        seq_t x = (seq_t)1;
        for (;;) {
            seq_t prod = A348244_prev * x;
            size_t pi = (size_t)prod;
            if (pi >= A348244_cnt_cap) {
                size_t nc = A348244_cnt_cap << 1;
                while (nc <= pi) nc <<= 1;
                A348244_cnt = (size_t *)realloc(A348244_cnt, nc * sizeof(size_t));
                memset(A348244_cnt + A348244_cnt_cap, 0, (nc - A348244_cnt_cap) * sizeof(size_t));
                A348244_cnt_cap = nc;
            }
            if ((seq_t)A348244_cnt[pi] < prod) {
                A348244_cnt[pi]++;
                break;
            }
            x++;
        }
        A348244_pos++;
        A348244_prev = x;
        if (x == (seq_t)1) {
            if (A348244_ones_len >= A348244_ones_cap) {
                A348244_ones_cap <<= 1;
                A348244_ones = (idx_t *)realloc(A348244_ones, A348244_ones_cap * sizeof(idx_t));
            }
            A348244_ones[A348244_ones_len++] = A348244_pos;
        }
    }
    return (seq_t)A348244_ones[n - 1];
}
