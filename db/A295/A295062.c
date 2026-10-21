/*
 * A295062  Solution of the complementary equation a(n) = 4*a(n-2) + b(n-2),
 *          where a(0) = 1, a(1) = 3, b(0) = 2, b(1) = 4, and (a(n)) and
 *          (b(n)) are increasing complementary sequences.
 * https://oeis.org/A295062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A295062 – Complementary equation a(n) = 4*a(n-2) + b(n-2)  */
/*  a(0)=1, a(1)=3, b(0)=2, b(1)=4, (a(n)) and (b(n)) are increasing complementary sequences.  */
/*  The function below uses a dynamic “mex” algorithm that scans for the smallest unused
 *  positive integer at each step.  The sequence is stored in static arrays,
 *  and the function is memoised so calls with arbitrary ordering of n are handled efficiently.  */

#include <stdlib.h>
#include <stddef.h>

/* Lookup helper: binary search for equality in a sorted array. */
static inline int
exists_in_seq(const seq_t *arr, size_t len, seq_t v)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (arr[mid] < v)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < len && arr[lo] == v);
}

/* Static storage for the sequences and the mex value. */
static seq_t *a_vals = NULL;
static seq_t *b_vals = NULL;
static size_t a_len = 0;          /* current number of a terms stored */
static size_t b_len = 0;          /* current number of b terms stored */
static seq_t next_candidate = 1;  /* next integer to test for the mex */

/* Initialise the sequences with the first two terms. */
static void
init_seq(void)
{
    a_vals = (seq_t *)malloc(2 * sizeof(seq_t));
    b_vals = (seq_t *)malloc(2 * sizeof(seq_t));
    if (!a_vals || !b_vals)
        exit(1);

    a_vals[0] = (seq_t)1;
    a_vals[1] = (seq_t)3;
    b_vals[0] = (seq_t)2;
    b_vals[1] = (seq_t)4;
    a_len = 2;
    b_len = 2;

    /* The smallest unused integer after the first four numbers is 5. */
    next_candidate = 1;
}

/* Append a new term to the a array, reallocating if necessary.  */
static void
append_a(seq_t val)
{
    seq_t *tmp = (seq_t *)realloc(a_vals, (a_len + 1) * sizeof(seq_t));
    if (!tmp)
        exit(1);
    a_vals = tmp;
    a_vals[a_len++] = val;
}

/* Append a new term to the b array, reallocating if necessary.  */
static void
append_b(seq_t val)
{
    seq_t *tmp = (seq_t *)realloc(b_vals, (b_len + 1) * sizeof(seq_t));
    if (!tmp)
        exit(1);
    b_vals = tmp;
    b_vals[b_len++] = val;
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (!a_vals)          /* first call – initialise */
        init_seq();

    if ((idx_t)n < a_len)
        return a_vals[n];

    /* Compute terms up to n.  */
    for (idx_t i = a_len; i <= n; ++i) {
        /*  a(i) = 4*a(i-2) + b(i-2)  */
        seq_t ai = 4 * a_vals[i - 2] + b_vals[i - 2];
        append_a(ai);

        /*  Find the smallest unused positive integer – the mex  */
        seq_t cand = next_candidate;
        while (exists_in_seq(a_vals, i + 1, cand) ||   /* includes the newly added a(i) */
               exists_in_seq(b_vals, i,     cand)) {  /* b has one element less */
            ++cand;
        }
        seq_t bi = cand;
        append_b(bi);

        /*  Prepare candidate for the next iteration  */
        next_candidate = bi + 1;
    }

    return a_vals[n];
}
