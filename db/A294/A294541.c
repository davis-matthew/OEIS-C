/*
 * A294541  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1), where a(0) = 1, a(1) = 2, b(0) = 3, and (a(n)) and (b(n))
 *          are increasing complementary sequences.
 * https://oeis.org/A294541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Complementary sequences A294541
   a(n) = a(n-1) + a(n-2) + b(n-1),
   a(0)=1, a(1)=2, b(0)=3.
   b(n) is the least positive integer not in {a(0)…a(n)} ∪ {b(0)…b(n-1)}. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *a_arr = NULL;          /* array holding a(0)…a(k)   */
static seq_t *b_arr = NULL;          /* array holding b(0)…b(k-1) */
static size_t a_cap   = 0;
static size_t b_cap   = 0;
static idx_t   a_len   = 0;          /* number of a terms stored  */
static idx_t   b_len   = 0;          /* number of b terms stored  */

static seq_t next_b_candidate = 4;   /* smallest integer ready to try for b */
static size_t ptrA = 0;              /* index in a_arr used for candidate test */

/* Ensure capacity of an array of seq_t.  */
static void ensure_cap(seq_t **arr, size_t *cap, size_t need)
{
    if (need > *cap) {
        size_t new_cap = (*cap) ? (*cap << 1) : 16;
        while (new_cap < need) new_cap <<= 1;
        *arr = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* Return the next unused integer for b(n).  */
static seq_t next_unused_b(void)
{
    seq_t cand = next_b_candidate;
    /* Advance `ptrA` until a_arr[ptrA] >= cand or we run out of a terms.  */
    while (ptrA < a_len && a_arr[ptrA] < cand) ++ptrA;

    /* If cand is already in a[], skip it.  */
    while (ptrA < a_len && a_arr[ptrA] == cand) {
        ++cand;
        while (ptrA < a_len && a_arr[ptrA] < cand) ++ptrA;
    }

    next_b_candidate = cand + 1;
    return cand;
}

/* Initialise the static arrays with the seed values.  */
static void init_seq(void)
{
    if (a_arr) return;  /* already initialised */

    ensure_cap(&a_arr, &a_cap, 16);
    ensure_cap(&b_arr, &b_cap, 16);

    a_arr[0] = (seq_t)1;
    a_arr[1] = (seq_t)2;
    a_len   = 2;

    b_arr[0] = (seq_t)3;
    b_len   = 1;

    ptrA = 0;
    next_b_candidate = 4;
}

/* Compute all terms up to index `n`.  */
static void compute_upto(idx_t n)
{
    init_seq();
    while (a_len - 1 < n) {
        /* Compute next b value.  */
        seq_t b_next = next_unused_b();
        ensure_cap(&b_arr, &b_cap, b_len + 1);
        b_arr[b_len++] = b_next;

        /* Compute next a value using the recurrence.  */
        seq_t a_prev1 = a_arr[a_len - 1];
        seq_t a_prev2 = a_arr[a_len - 2];
        seq_t a_new   = a_prev1 + a_prev2 + b_next;

        ensure_cap(&a_arr, &a_cap, a_len + 1);
        a_arr[a_len++] = a_new;
    }
}

/* Public function required by the judge.  */
seq_t a(idx_t n)
{
    compute_upto(n);
    return a_arr[n];
}
