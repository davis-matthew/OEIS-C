/*
 * A293076  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-2), where a(0) = 1, a(1) = 3, b(0) = 2, b(1) = 4.
 * https://oeis.org/A293076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-2) + b(n-2),
   a(0) = 1, a(1) = 3, b(0) = 2, b(1) = 4 */

/* Includes are minimal and valid for C11 and C++17 */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/*  Dynamic arrays holding a(n) and b(n)                               */
/* ------------------------------------------------------------------ */
static seq_t *A = NULL;            /* array of a values */
static seq_t *B = NULL;            /* array of b values */
static size_t A_cap = 0;           /* allocated capacity of A/B */
static idx_t  last_computed = 0;   /* largest index computed for a() */

/* Smallest unused integer, kept between calls */
static seq_t next_candidate = 1;

/* ------------------------------------------------------------------ */
/*  Utility: binary search on a sorted array (size elements).          */
/* ------------------------------------------------------------------ */
static int binary_search(const seq_t *arr, size_t size, seq_t value)
{
    size_t lo = 0, hi = size;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (arr[mid] < value) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return (lo < size && arr[lo] == value);
}

/* ------------------------------------------------------------------ */
/*  Ensure that arrays A and B have capacity >= n+1.                   */
/* ------------------------------------------------------------------ */
static void ensure_capacity(idx_t n)
{
    if ((idx_t)(A_cap) >= n + 1) return;

    size_t new_cap = A_cap ? (A_cap << 1) : 8;
    while ((idx_t)new_cap <= n) new_cap <<= 1;

    seq_t *tmpA = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
    seq_t *tmpB = (seq_t *)realloc(B, new_cap * sizeof(seq_t));
    if (!tmpA || !tmpB) exit(1);

    /* new elements are zero-initialised */
    memset(tmpA + A_cap, 0, (new_cap - A_cap) * sizeof(seq_t));
    memset(tmpB + A_cap, 0, (new_cap - A_cap) * sizeof(seq_t));

    A = tmpA;
    B = tmpB;
    A_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Initialise static data on first use.                               */
/* ------------------------------------------------------------------ */
static void init(void)
{
    if (A) return;          /* already initialised */

    A_cap = 8;
    A = (seq_t *)malloc(A_cap * sizeof(seq_t));
    B = (seq_t *)malloc(A_cap * sizeof(seq_t));
    if (!A || !B) exit(1);
    memset(A, 0, A_cap * sizeof(seq_t));
    memset(B, 0, A_cap * sizeof(seq_t));

    /* Base values */
    A[0] = (seq_t)1;  A[1] = (seq_t)3;
    B[0] = (seq_t)2;  B[1] = (seq_t)4;
    last_computed = 1;
    next_candidate = 1;   /* will be advanced in the loop */
}

/* ------------------------------------------------------------------ */
/*  Ensure that a[0..n] and b[0..n] (where defined) have been         */
/*  computed.                                                            */
/* ------------------------------------------------------------------ */
static void ensure_computed(idx_t n)
{
    init();

    if (n <= last_computed) return;
    ensure_capacity(n);

    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* a(i) = a(i-1) + a(i-2) + b(i-2)   (b(i-2) exists for i>=2) */
        seq_t aval = A[i-1] + A[i-2];
        if (i >= 2) aval += B[i-2];
        A[i] = aval;

        /* Compute b(i) for i >= 2 */
        if (i >= 2) {
            /* Increment candidate until it is not present in a[0..i] or b[0..i-1] */
            while (binary_search(A, i+1, next_candidate) || binary_search(B, i, next_candidate)) {
                next_candidate++;
            }
            B[i] = next_candidate;
            next_candidate++;
        }
    }
    last_computed = n;
}

/* ------------------------------------------------------------------ */
/*  Public function required by the build.                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    ensure_computed(n);
    return A[n];
}
