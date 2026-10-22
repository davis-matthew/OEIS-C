/*
 * A357529  Triangular numbers k such that 2*k cannot be expressed as a sum of
 *          two distinct triangular numbers.
 * https://oeis.org/A357529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th triangular number k (k = T_i = i*(i+1)/2) such
 *            * that 2*k cannot be expressed as the sum of two distinct
 *            triangular numbers. * Implementation: maintain a list of all
 *            triangular numbers generated so far * and a list of the accepted
 *            ones. For each new triangular number k, * test th...
 * Range:     u8 n <= 10, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *tri = NULL;         /* all triangular numbers generated */
static size_t tri_cap = 0;
static size_t tri_len = 0;

static seq_t *accepted = NULL;    /* accepted triangular numbers */
static size_t acc_cap = 0;
static size_t acc_len = 0;

static idx_t next_m = 0;          /* next index i of triangular number to examine */

/* Ensure that the triangular number with index m is already stored. */
static void ensure_tri(idx_t m)
{
    while (tri_len <= (size_t)m) {
        seq_t val = ((seq_t)tri_len * ((seq_t)tri_len + 1)) / 2;
        if (tri_len == tri_cap) {
            tri_cap = tri_cap ? tri_cap * 2 : 64;
            tri = (seq_t *)realloc(tri, tri_cap * sizeof(seq_t));
        }
        tri[tri_len++] = val;
    }
}

/* Ensure that all triangular numbers up to value v (inclusive) are stored. */
static void ensure_tri_up_to(seq_t v)
{
    while (tri_len == 0 || tri[tri_len - 1] < v) {
        seq_t val = ((seq_t)tri_len * ((seq_t)tri_len + 1)) / 2;
        if (tri_len == tri_cap) {
            tri_cap = tri_cap ? tri_cap * 2 : 64;
            tri = (seq_t *)realloc(tri, tri_cap * sizeof(seq_t));
        }
        tri[tri_len++] = val;
    }
}

/* Ensure capacity for the accepted array before inserting a new element. */
static void ensure_accept(size_t need)
{
    if (acc_len == acc_cap) {
        acc_cap = acc_cap ? acc_cap * 2 : 64;
        accepted = (seq_t *)realloc(accepted, acc_cap * sizeof(seq_t));
    }
}

/* Return the n-th element of the sequence.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 1, defensive guard */

    /* Produce accepted numbers until we have at least n of them. */
    while (acc_len < (size_t)n) {
        /* Generate the next triangular number T = next_m*(next_m+1)/2. */
        ensure_tri(next_m);
        seq_t target = tri[next_m] * 2;    /* 2*k */

        /* Make sure we can examine all triangular numbers up to target. */
        ensure_tri_up_to(target);

        /* Two‑pointer search for distinct triangular numbers summing to target. */
        size_t l = 0;
        size_t r = tri_len - 1;
        bool representable = false;
        while (l < r) {
            seq_t sum = tri[l] + tri[r];
            if (sum == target) {
                representable = true; /* found representation with distinct indices */
                break;
            }
            if (sum < target) {
                l++;
            } else {
                if (r == 0) break; /* avoid underflow; r is size_t */
                r--;
            }
        }

        if (!representable) {
            ensure_accept(acc_len + 1);
            accepted[acc_len++] = tri[next_m];
        }
        next_m++;
    }

    return accepted[n - 1];
}
