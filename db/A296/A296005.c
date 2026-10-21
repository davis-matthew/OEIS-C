/*
 * A296005  Solution of the complementary equation a(n) = a(0)*b(n-1) +
 *          a(1)*b(n-2) + ... + a(n-1)*b(0), where a(0) = 2, a(1) = 3, b(0) =
 *          1, and (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A296005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A296005: a(0)=2, a(1)=3, b(0)=1
 * a(n) = sum_{k=0}^{n-1} a(k) * b(n-1-k)
 * a and b are complementary increasing sequences. */

/* Required headers for static allocation and booleans */
#include <stdlib.h>
#include <stdbool.h>

/* Static state: sequence arrays and bookkeeping */
static seq_t *A = NULL, *B = NULL;    /* sequence values */
static size_t A_cap = 0, B_cap = 0;   /* allocated capacities */
static size_t A_len = 0, B_len = 0;   /* current lengths */
static seq_t next_candidate = 2;      /* next value to test for b */
static bool initialized = false;     /* init flag */

/* --------------------------------------------------------------------
 * Simple dynamic array growth helper.  Ensures *cap >= need.       */
static void grow_array(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t newcap = (*cap) ? (*cap) << 1 : 8;
    while (newcap < need) newcap <<= 1;
    *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    *cap = newcap;
}

/* --------------------------------------------------------------------
 * Binary search for membership in the array A (which is always sorted). */
static bool a_contains(seq_t val)
{
    size_t lo = 0, hi = A_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        seq_t midv = A[mid];
        if (midv == val) return true;
        if (midv < val) lo = mid + 1;
        else hi = mid;
    }
    return false;
}

/* --------------------------------------------------------------------
 * Initialise the static data structures once.                           */
static void init(void)
{
    if (initialized) return;
    A_cap = 16; B_cap = 16;
    A = (seq_t *)malloc(A_cap * sizeof(seq_t));
    B = (seq_t *)malloc(B_cap * sizeof(seq_t));

    /* base values */
    A[0] = 2;  A[1] = 3;
    B[0] = 1;

    A_len = 2; B_len = 1;
    next_candidate = 2;
    initialized = true;
}

/* --------------------------------------------------------------------
 * Extend the sequences so that A[n] is computed (and B[] up to n-1).   */
static void ensure_upto(idx_t n)
{
    init();
    while (A_len <= (size_t)n) {
        /* create B entries until B_len == A_len */
        while (B_len < A_len) {
            seq_t cand = next_candidate;
            while (a_contains(cand)) ++cand;      /* skip numbers already in A */
            if (B_len >= B_cap) grow_array(&B, &B_cap, B_len + 1);
            B[B_len++] = cand;                   /* store next b value */
            next_candidate = cand + 1;           /* next candidate starts after */
        }

        /* compute a[A_len] = sum_{k=0}^{A_len-1} A[k] * B[A_len-1-k] */
        seq_t sum = 0;
        idx_t i = A_len;                       /* index to fill */
        for (idx_t k = 0; k < i; ++k) {
            sum += A[k] * B[i - 1 - k];
        }
        if (A_len >= A_cap) grow_array(&A, &A_cap, A_len + 1);
        A[A_len++] = sum;
    }
}

/* --------------------------------------------------------------------
 * External function required by the problem statement.                */
seq_t a(idx_t n)
{
    if ((idx_t)-1 == n) return 0;           /* defensive: avoid underflow */
    ensure_upto(n);
    return A[(size_t)n];
}
