/*
 * A294551  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1) + b(n-2) + 1, where a(0) = 1, a(1) = 2, b(0) = 3, and
 *          (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A294551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n-1) + b(n-2) + 1 with a(0)=1,
 *            a(1)=2, b(0)=3; a() and b() are complementary increasing
 *            sequences
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   contains_in_A
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/contains_in_A.h"

/* static state for memoisation ------------------------------------------------- */
static seq_t *A = NULL;                /* values of a(i)                         */
static seq_t *B = NULL;                /* values of b(i)                         */
static size_t A_cap   = 0;              /* current storage capacity (elements)    */
static idx_t cur_n   = 0;               /* highest index that has been computed    */
static seq_t next_b_candidate = 3;      /* smallest integer still potentially a b */

/* ----------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------- */
/* Ensure arrays are large enough to hold indices up to idx (inclusive)          */
static void ensure_capacity(idx_t idx)
{
    if ((size_t)idx < A_cap)
        return;
    size_t newcap = A_cap ? A_cap * 2 : 4;
    while (newcap <= (size_t)idx)
        newcap <<= 1;
    A = (seq_t *)realloc(A, newcap * sizeof(seq_t));
    B = (seq_t *)realloc(B, newcap * sizeof(seq_t));
    A_cap = newcap;
}

/* ----------------------------------------------------------------------------- */
/* Initialise base values once: a(0)=1, a(1)=2, b(0)=3, b(1)=4                 */
static void init(void)
{
    if (A)                /* already initialised */
        return;

    A_cap = 4;
    A = (seq_t *)calloc(A_cap, sizeof(seq_t));
    B = (seq_t *)calloc(A_cap, sizeof(seq_t));

    A[0] = (seq_t)1;
    A[1] = (seq_t)2;
    B[0] = (seq_t)3;

    /* determine b(1) = least positive integer not in a[0..1] */
    do
        ++next_b_candidate;
    while (contains_in_A(A, 2, next_b_candidate));   /* gives 4 */

    B[1] = next_b_candidate;        /* B[1] = 4 */

    cur_n = 1;                      /* a(0) and a(1) are ready */
}

/* ----------------------------------------------------------------------------- */
/* Return a(n) for any n ≥ 0, memoising intermediate values                    */
seq_t a(idx_t n)
{
    init();                         /* make sure the tables exist */

    if (n < cur_n)                  /* already computed */
        return A[n];

    for (idx_t k = cur_n + 1; k <= n; ++k) {
        ensure_capacity(k);

        /* recurrence: a(k) = a(k-1)+a(k-2)+b(k-1)+b(k-2)+1           */
        A[k] = A[k-1] + A[k-2] + B[k-1] + B[k-2] + (seq_t)1;

        /* pick the next b(k) : smallest integer not yet in a(0..k) */
        do
            ++next_b_candidate;
        while (contains_in_A(A, (size_t)(k + 1), next_b_candidate));

        B[k] = next_b_candidate;
    }

    cur_n = n;
    return A[n];
}
