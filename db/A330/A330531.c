/*
 * A330531  Lexicographically earliest sequence of distinct positive integers
 *          such that the product of two consecutive terms is always divisible
 *          by 6.
 * https://oeis.org/A330531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 159, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: build the lexicographically earliest permutation of the
 * natural numbers in which the product of every two consecutive terms
 * is divisible by 6.  The sequence is constructed greedily: at each
 * step choose the smallest unused integer that satisfies the adjacency
 * rule determined by the previous term (its residue modulo 6).  This
 * produces the unique permutation whose first few terms are
 *
 * 1, 6, 2, 3, 4, 9, 8, 12, 5, 18, 7, 24, …  (OEIS A330531)
 */

#include <stdlib.h>
#include <stdbool.h>

/* --- global state, retained between calls -------------------------------- */

static seq_t *seq = NULL;          /* array of generated sequence values   */
static size_t seq_cap = 0;         /* capacity of the seq array            */
static size_t seq_len = 0;         /* number of values currently stored    */

static bool *used = NULL;          /* boolean array: true if number used   */
static size_t used_cap = 0;        /* capacity of the used array           */

/* pointers to the smallest unused integers in each residue class   */
static size_t ptr_any = 1;   /* any integer                          */
static size_t ptr2   = 2;   /* even numbers (≡0 mod2)               */
static size_t ptr3   = 3;   /* multiples of 3                        */
static size_t ptr6   = 6;   /* multiples of 6                        */

/* ----------------------------------------------------------------------- */

static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < need) new_cap *= 2;
    seq = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

static void ensure_used_capacity(size_t idx)
{
    if (idx < used_cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 128;
    while (new_cap <= idx) new_cap *= 2;
    used = (bool*)realloc(used, new_cap * sizeof(bool));
    for (size_t i = used_cap; i < new_cap; ++i) used[i] = false;
    used_cap = new_cap;
}

/* find the next unused number in the class beginning at *ptr,
 * incrementing by step each trial.  *ptr is updated to that number. */
static size_t find_next_unused(size_t *ptr, size_t step)
{
    size_t i = *ptr;
    for (;; i += step) {
        if (i >= used_cap) ensure_used_capacity(i);
        if (!used[i]) { *ptr = i; return i; }
    }
}

/* ----------------------------------------------------------------------- */

/* External function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined case – safety */

    if (!seq) {                    /* first call: initialise state */
        seq_cap = 64;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        seq_len = 0;
        used_cap = 128;
        used = (bool*)malloc(used_cap * sizeof(bool));
        for (size_t i = 0; i < used_cap; ++i) used[i] = false;
    }

    /* generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        ensure_seq_capacity(seq_len + 1);

        if (seq_len == 0) {                /* first term is always 1   */
            seq[0] = (seq_t)1;
            seq_len = 1;
            ensure_used_capacity(1);
            used[1] = true;
            continue;
        }

        seq_t prev = seq[seq_len - 1];
        idx_t mod6 = (idx_t)(prev % (seq_t)6);
        idx_t mod2 = (idx_t)(prev % (seq_t)2);
        idx_t mod3 = (idx_t)(prev % (seq_t)3);

        size_t cand_val;
        if (mod6 == 0) {
            cand_val = find_next_unused(&ptr_any, 1);   /* any integer   */
        } else if (mod2 == 0) {                       /* even, not 3   */
            cand_val = find_next_unused(&ptr3, 3);      /* need multiple of 3 */
        } else if (mod3 == 0) {                       /* odd multiple of 3 */
            cand_val = find_next_unused(&ptr2, 2);      /* need even      */
        } else {                                      /* neither 2 nor 3 */
            cand_val = find_next_unused(&ptr6, 6);      /* need multiple of 6 */
        }

        seq[seq_len++] = (seq_t)cand_val;
        ensure_used_capacity(cand_val);
        used[cand_val] = true;
    }

    return seq[n - 1];
}
