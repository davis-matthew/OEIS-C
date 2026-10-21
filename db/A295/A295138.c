/*
 * A295138  Solution of the complementary equation a(n) = 3*a(n-2) + b(n-1),
 *          where a(0) = 1, a(1) = 2, b(0) = 3, and (a(n)) and (b(n)) are
 *          increasing complementary sequences.
 * https://oeis.org/A295138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   next_unused
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A295138: a(n) = 3*a(n-2) + b(n-1) for n ≥ 2
 *   a(0) = 1, a(1) = 2, b(0) = 3
 *   (a(n)) and (b(n)) are the increasing complementary sequences of positive integers.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_unused.h"

seq_t a(idx_t n)
{
    /* ---------- static storage for the sequences ---------- */
    static seq_t *A = NULL;      /* a() values   */
    static seq_t *B = NULL;      /* b() values   */
    static size_t A_len = 0, B_len = 0;
    static size_t A_cap = 0, B_cap = 0;

    /* ---------- static data for finding next unused integer ---------- */
    static seq_t next_candidate = 1;    /* smallest integer to test */
    static size_t idx_A_use = 0, idx_B_use = 0;   /* scanning positions */

    /* ---------- helper: ensure capacity ---------- */
    if (A_cap == 0) {
        A_cap = 4;
        A = (seq_t *)malloc(A_cap * sizeof(seq_t));
    }
    if (B_cap == 0) {
        B_cap = 4;
        B = (seq_t *)malloc(B_cap * sizeof(seq_t));
    }

    /* ---------- initialization of first terms ---------- */
    if (A_len == 0) {
        A[0] = 1;
        A[1] = 2;
        A_len = 2;
    }
    if (B_len == 0) {
        B[0] = 3;
        B_len = 1;
    }

    /* ---------- generate sequences up to required n ---------- */
    while (A_len <= (size_t)n) {
        size_t need_b_index = A_len - 1;
        while (B_len <= need_b_index) {
            seq_t val = next_unused(A, A_len, B, B_len,
                                    &idx_A_use, &idx_B_use, &next_candidate);
            if (B_len == B_cap) {
                B_cap *= 2;
                B = (seq_t *)realloc(B, B_cap * sizeof(seq_t));
            }
            B[B_len++] = val;
        }

        seq_t new_a;
        if (A_len < 2) {            /* should not happen for n>1 */
            new_a = (A_len == 0) ? 1 : 2;
        } else {
            new_a = ((seq_t)3 * A[A_len - 2]) + B[A_len - 1];
        }

        if (A_len == A_cap) {
            A_cap *= 2;
            A = (seq_t *)realloc(A, A_cap * sizeof(seq_t));
        }
        A[A_len++] = new_a;
    }

    return A[(size_t)n];
}
