/*
 * A297997  Solution (b(n)) of the near-complementary equation a(n) =
 *          a(1)*b(n-1) - a(0)*b(n-2) + n, where a(0) = 1, a(1) = 2, b(0) = 3,
 *          b(1) = 4, and (b(n)) is the increasing sequence of positive
 *          integers not in (a(n)). See Comments.
 * https://oeis.org/A297997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 159, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A297997:  b(n) are the positive integers not occurring in the sequence
   a(n) defined recursively by
         a(0) = 1,  a(1) = 2,
         a(n) = a(1)*b(n-1) - a(0)*b(n-2) + n   (n ≥ 2)
   where b(n) is the n-th smallest positive integer that is not in {a(k)}.
   The function below returns b(n) (i.e., the OEIS sequence A297997).
*/

#include <stdlib.h>
#include <string.h>
#include <stddef.h>

static seq_t *b_vals = NULL;   /* stored b-values */
static idx_t  b_len  = 0;      /* number of stored b-values   */
static idx_t  b_cap  = 0;      /* allocated capacity         */

static uint8_t *present = NULL;   /* 1 if a-value has appeared   */
static size_t  present_cap = 0;   /* allocated capacity (indices) */

static idx_t   next_candidate = 1;   /* next integer to test for b */

/* -- helper: ensure b_vals has capacity >= need --------------------------------- */
static void grow_b(idx_t need)
{
    if ((idx_t)present_cap <= need) return; /* no need if already enough */
    size_t new_cap = b_cap > 0 ? (size_t)b_cap << 1 : 1;
    while ((idx_t)new_cap <= need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(b_vals, new_cap * sizeof(*b_vals));
    if (!tmp) return;      /* allocation failure – caller will treat as 0 */
    b_vals = tmp;
    b_cap  = (idx_t)new_cap;
}

/* -- helper: ensure present has capacity >= need --------------------------------- */
static void ensure_present_capacity(idx_t need)
{
    if ((idx_t)present_cap > need) return;
    size_t new_cap = present_cap > 0 ? present_cap << 1 : 1;
    while (new_cap <= need)
        new_cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(present, new_cap * sizeof(*present));
    if (!tmp) return;      /* allocation failure – values beyond will be ignored */
    /* zero the new portion */
    memset(tmp + present_cap, 0, (new_cap - present_cap) * sizeof(*tmp));
    present = tmp;
    present_cap = new_cap;
}

/* -- main routine --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    const seq_t a0 = 1;
    const seq_t a1 = 2;

    /* initialise on first call */
    if (b_len == 0) {
        /* start with enough room for indices 0..2 (a0,a1 and needed room) */
        ensure_present_capacity(3);
        present[1] = 1;
        present[2] = 1;
        next_candidate = 3;

        /* first two b-values */
        seq_t b0 = next_candidate;   /* 3 */
        ++next_candidate;
        seq_t b1 = next_candidate;   /* 4 */
        ++next_candidate;

        grow_b(1);      /* capacity for indices 0 and 1 */
        b_vals[0] = b0;
        b_vals[1] = b1;
        b_len    = 2;
    }

    /* if already computed, just return */
    if (n < b_len) return b_vals[n];

    /* compute up to n */
    for (idx_t k = b_len; k <= n; ++k) {
        /* compute a(k) = a1*b(k-1) - a0*b(k-2) + k */
        seq_t a_k = (seq_t)2 * b_vals[k - 1] - a0 * b_vals[k - 2] + (seq_t)k;

        /* mark a_k as present – enlarge array if necessary */
        ensure_present_capacity((idx_t)(a_k) + 1);
        if ((idx_t)a_k < present_cap)
            present[(size_t)a_k] = 1;

        /* find next missing integer for b(k) */
        while (1) {
            if (next_candidate >= present_cap)
                ensure_present_capacity(next_candidate + 1);
            if (!present[(size_t)next_candidate])
                break;
            ++next_candidate;
        }
        seq_t b_k = (seq_t)next_candidate;
        ++next_candidate;

        /* store b_k */
        grow_b(k);
        b_vals[k] = b_k;
    }

    b_len = n + 1;
    return b_vals[n];
}
