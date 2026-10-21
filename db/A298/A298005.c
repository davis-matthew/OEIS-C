/*
 * A298005  Solution b( ) of the complementary equation a(n) = a(1)*b(n-1) -
 *          a(0)*b(n-2) + 4*n, where a(0) = 1, a(1) = 2, b(0) = 3, b(1) = 4,
 *          and (b(n)) is the increasing sequence of positive integers not in
 *          (a(n)). See Comments.
 * https://oeis.org/A298005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(1)*b(n‑1) – a(0)*b(n‑2) + 4*n (complementary sequence
 *            b) * a(0) = 1, a(1) = 2, b(0) = 3, b(1) = 4. * The function
 *            returns the nth term of b (the complementary sequence)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static storage for the two complementary sequences */
static seq_t *A_vals = NULL;            /* a(0), a(1), ... (only a0,a1 needed) */
static size_t A_len  = 0, A_cap  = 0;
static seq_t *B_vals = NULL;            /* b(0), b(1), ... */
static size_t B_len  = 0, B_cap  = 0;

/* the next positive integer not yet assigned to either sequence */
static seq_t Next_num = (seq_t)5;
/* index of the next a value that still needs to be computed */
static size_t Next_a_idx = 2;

/* initialise the sequences once (idempotent) */
static void init_sequences(void)
{
    if (A_vals) return;            /* already initialised */

    A_cap = 16;
    B_cap = 16;
    A_vals = (seq_t*)malloc(A_cap * sizeof(seq_t));
    B_vals = (seq_t*)malloc(B_cap * sizeof(seq_t));

    /* a(0) = 1, a(1) = 2 */
    A_vals[0] = (seq_t)1;
    A_vals[1] = (seq_t)2;
    A_len   = 2;

    /* b(0) = 3, b(1) = 4 */
    B_vals[0] = (seq_t)3;
    B_vals[1] = (seq_t)4;
    B_len    = 2;

    Next_num   = (seq_t)5;
    Next_a_idx = 2;
}

/* push a value onto sequence A */
static void push_A(seq_t v)
{
    if (A_len == A_cap) {
        A_cap = A_cap ? A_cap * 2 : 8;
        A_vals = (seq_t*)realloc(A_vals, A_cap * sizeof(seq_t));
    }
    A_vals[A_len++] = v;
}

/* push a value onto sequence B */
static void push_B(seq_t v)
{
    if (B_len == B_cap) {
        B_cap = B_cap ? B_cap * 2 : 8;
        B_vals = (seq_t*)realloc(B_vals, B_cap * sizeof(seq_t));
    }
    B_vals[B_len++] = v;
}

/* ---- required function ---- */
seq_t a(idx_t n)
{
    init_sequences();

    size_t target = (size_t)n;
    while (B_len <= target) {
        /* compute candidate_a if we have enough b values */
        seq_t cand_a = 0;
        int have_a = 0;
        if (Next_a_idx >= 2 && B_len >= Next_a_idx) {
            seq_t a0 = A_vals[0];
            seq_t a1 = A_vals[1];
            seq_t b1 = B_vals[Next_a_idx - 1];
            seq_t b0 = B_vals[Next_a_idx - 2];
            cand_a = a1 * b1 - a0 * b0 + (seq_t)4 * (seq_t)Next_a_idx;
            have_a = 1;
        }

        if (have_a && cand_a == Next_num) {
            /* this integer is the next a value */
            push_A(Next_num);
            Next_a_idx++;
        } else if (!have_a || Next_num < cand_a) {
            /* otherwise it is the next b value */
            push_B(Next_num);
        } else {
            /* cand_a < Next_num – should not normally happen.
               Skip to the missing a value. */
            Next_num = cand_a;
            continue;
        }
        Next_num++;
    }

    return B_vals[target];
}
