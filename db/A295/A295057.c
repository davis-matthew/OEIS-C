/*
 * A295057  Solution of the complementary equation a(n) = 2*a(n-1) + b(n-1),
 *          where a(0) = 2, a(1) = 5, b(0) = 1, and (a(n)) and (b(n)) are
 *          increasing complementary sequences.
 * https://oeis.org/A295057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=2, a(1)=5, b(0)=1, increasing complementary sequences with
 *            a(n)=2*a(n-1)+b(n-1) and b(n) the least positive integer not yet
 *            used
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* static storage for the sequences and the used‑b bitmap */
static seq_t *A = NULL;
static size_t A_len = 0, A_cap = 0;
static seq_t *B = NULL;
static size_t B_len = 0, B_cap = 0;
static bool *used_b = NULL;
static size_t used_b_cap = 0;
static size_t last_b = 0;            /* last used value of b */

/* grow an array of seq_t to at least 'needed' elements */
static void grow_array(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = *cap ? *cap : 1;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        *arr = tmp;
        *cap = new_cap;
    }
}

/* ensure used_b can index 'idx', expanding and zero‑initialising as needed */
static void ensure_used_b(size_t idx)
{
    if (idx >= used_b_cap) {
        size_t new_cap = used_b_cap ? used_b_cap : 1;
        while (new_cap <= idx) new_cap <<= 1;
        bool *tmp = (bool *)realloc(used_b, new_cap * sizeof(bool));
        if (!tmp) abort();
        /* zero the newly allocated portion */
        memset(tmp + used_b_cap, 0, (new_cap - used_b_cap) * sizeof(bool));
        used_b = tmp;
        used_b_cap = new_cap;
    }
}

/* binary search in A[0..upto-1] for value val */
static bool is_in_a(seq_t val, size_t upto)
{
    size_t lo = 0, hi = upto;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (A[mid] < val)
            lo = mid + 1;
        else if (A[mid] > val)
            hi = mid;
        else
            return true;
    }
    return false;
}

/* required entry point */
seq_t a(idx_t n)
{
    /* one‑time initialisation */
    if (!A) {
        A_cap = 16;
        A = (seq_t *)calloc(A_cap, sizeof(seq_t));
        B_cap = 16;
        B = (seq_t *)calloc(B_cap, sizeof(seq_t));
        used_b_cap = 0;
        used_b = NULL;

        /* base terms */
        A_len = 1;
        A[0] = (seq_t)2;      /* a(0) */
        B_len = 1;
        B[0] = (seq_t)1;      /* b(0) */
        last_b = 1;
        ensure_used_b(1);
        used_b[1] = true;     /* b(0) is already used */
    }

    /* compute terms up to required index n */
    while (A_len <= (size_t)n) {
        /* compute next a(n) */
        seq_t a_prev = A[A_len - 1];
        seq_t b_prev = B[B_len - 1];
        seq_t a_val = (seq_t)2 * a_prev + b_prev;

        grow_array(&A, &A_cap, A_len + 1);
        A[A_len++] = a_val;

        /* compute next b(n) */
        size_t cand = last_b;
        do {
            cand++;
            ensure_used_b(cand);
        } while (used_b[cand] || is_in_a((seq_t)cand, A_len - 1));

        seq_t b_val = (seq_t)cand;
        grow_array(&B, &B_cap, B_len + 1);
        B[B_len++] = b_val;

        used_b[cand] = true;
        last_b = cand;
    }

    return A[n];
}
