/*
 * A296276  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1)*b(n), where a(0) = 2, a(1) = 4, b(0) = 1, b(1) = 3, b(2) =
 *          5, and (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A296276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n-1)*b(n) for n ≥ 2 * with a(0)=2,
 *            a(1)=4 * b(0)=1, b(1)=3, b(2)=5 * The sequences a() and b() are
 *            the unique increasing complementary * sequences satisfying the
 *            recurrence
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 38, u64 n <= 85, u128 n <= 177,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* size_t */
#include <stdbool.h>    /* bool */

/* Limits: enough to handle n up to 10 000 comfortably. */
#define MAX_N     50001          /* maximum a-terms to cache */
#define MAX_USED  (MAX_N * 3)    /* space for used values (a and b together) */

/* Global caches */
static seq_t A[MAX_N];           /* a-values */
static seq_t B[MAX_N];           /* b-values */
static seq_t USED[MAX_USED];     /* values already used by a or b */
static size_t used_len = 0;      /* number of used values stored */
static size_t generated = 0;     /* number of a-values that have been generated */

/* ------------------------------------------------------------------ */
/* Helper: is this value already used? (linear search, small array)   */
/* ------------------------------------------------------------------ */
static inline bool is_used(const seq_t val, size_t len)
{
    for (size_t i = 0; i < len; ++i) {
        if (USED[i] == val) {
            return true;
        }
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Initialise the very first values of a() and b()                     */
/* ------------------------------------------------------------------ */
static void init(void)
{
    /* a(0) = 2, a(1) = 4, b(0)=1, b(1)=3, b(2)=5 */
    A[0] = (seq_t)2;
    A[1] = (seq_t)4;
    B[0] = (seq_t)1;
    B[1] = (seq_t)3;
    B[2] = (seq_t)5;

    /* mark the initial used values */
    USED[0] = (seq_t)1;   /* b(0) */
    USED[1] = (seq_t)2;   /* a(0) */
    USED[2] = (seq_t)3;   /* b(1) */
    USED[3] = (seq_t)4;   /* a(1) */
    USED[4] = (seq_t)5;   /* b(2) */
    used_len = 5;

    /* compute a(2) = a(0)+a(1)+b(1)*b(2) = 2+4+3*5 = 21 */
    seq_t a2 = A[0] + A[1] + B[1] * B[2];
    A[2] = a2;
    USED[used_len++] = a2;

    generated = 3;  /* indices 0,1,2 have been generated */
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (generated == 0) {
        init();
    }

    /* generate terms up to n if they are not yet generated */
    while (generated <= n) {
        size_t i = generated;      /* next index to generate (>=3) */

        /* find the smallest unused integer > b(i-1) to become b(i) */
        seq_t cand = B[i - 1] + (seq_t)1;
        while (is_used(cand, used_len)) {
            cand = cand + (seq_t)1;
        }
        B[i] = cand;
        USED[used_len++] = cand;   /* mark as used */

        /* compute a(i) = a(i-1) + a(i-2) + b(i-1)*b(i) */
        seq_t prod = B[i - 1] * B[i];
        seq_t sum  = A[i - 1] + A[i - 2] + prod;
        A[i] = sum;
        USED[used_len++] = sum;    /* mark as used */

        ++generated;
    }

    return A[n];
}
