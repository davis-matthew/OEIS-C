/*
 * A295053  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(0) + b(1) + ... + b(n-1), where a(0) = 1, a(1) = 2, b(0) = 3,
 *          and (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A295053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(0) + b(1) + ... + b(n-1), a(0) = 1,
 *            a(1) = 2, b(0) = 3. This implementation builds the sequences
 *            iteratively, keeping a running sum of the b sequence and
 *            generating the next missing integer for b by skipping values
 *            already used in a or b
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static state used for memoisation */
static seq_t *a_arr  = NULL;   /* array of a(n) values */
static idx_t a_used  = 0;      /* number of a values currently stored */
static idx_t a_cap   = 0;      /* allocated capacity for a_arr */

static seq_t *b_arr  = NULL;   /* array of b(n) values */
static idx_t b_used  = 0;      /* number of b values currently stored */
static idx_t b_cap   = 0;      /* allocated capacity for b_arr */

static seq_t sum_b      = 0;   /* sum of b(0)...b(current) */
static seq_t next_candidate = 1; /* next integer to test for b */

static idx_t ptr_a_scan = 0;   /* scan pointer in a_arr for membership test */
static idx_t ptr_b_scan = 0;   /* scan pointer in b_arr for membership test */

/* ensure that an array has at least |need| elements allocated */
static void ensure_capacity(idx_t need, seq_t **arr, idx_t *cap)
{
    if (*cap < need) {
        idx_t newcap = (*cap) ? (*cap) * 2 : 4;
        while (newcap < need) newcap *= 2;
        *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        *cap = newcap;
    }
}

/* generate the next b value (b[b_used]) */
static void compute_next_b(void)
{
    seq_t val = next_candidate;

    while (1) {
        /* skip values present in a */
        while (ptr_a_scan < a_used && a_arr[ptr_a_scan] < val) ++ptr_a_scan;
        if (ptr_a_scan < a_used && a_arr[ptr_a_scan] == val) { ++val; continue; }

        /* skip values present in b */
        while (ptr_b_scan < b_used && b_arr[ptr_b_scan] < val) ++ptr_b_scan;
        if (ptr_b_scan < b_used && b_arr[ptr_b_scan] == val) { ++val; continue; }

        break;  /* val is unused */
    }

    ensure_capacity(b_used + 1, &b_arr, &b_cap);
    b_arr[b_used++] = val;
    sum_b += val;
    next_candidate = val + 1;
}

/* initialise the static state on first call */
static void initialise(void)
{
    if (a_used == 0) {                /* not yet initialised */
        ensure_capacity(2, &a_arr, &a_cap);
        a_arr[0] = (seq_t)1;
        a_arr[1] = (seq_t)2;
        a_used  = 2;

        ensure_capacity(1, &b_arr, &b_cap);
        b_arr[0] = (seq_t)3;
        b_used  = 1;

        sum_b = (seq_t)3;             /* b(0) */
        next_candidate = (seq_t)1;    /* start searching from 1 */

        ptr_a_scan = 0;
        ptr_b_scan = 0;
    }
}

/* the required sequence function */
seq_t a(idx_t n)
{
    initialise();

    if (n < a_used) return a_arr[n];

    /* extend the arrays up to n */
    ensure_capacity(n + 1, &a_arr, &a_cap);
    ensure_capacity(n + 1, &b_arr, &b_cap);

    for (idx_t i = a_used; i <= n; ++i) {
        /* ensure we have b[0..i-1] before computing a[i] */
        if (i > 1) {
            while (b_used < i) compute_next_b();
        }

        seq_t ai;
        if (i == 0) ai = (seq_t)1; /* already handled, but keep for completeness */
        else if (i == 1) ai = (seq_t)2;
        else
            ai = a_arr[i-1] + a_arr[i-2] + sum_b;

        a_arr[a_used++] = ai;

        /* after computing a[i], generate b[i] for future use */
        compute_next_b();
    }

    return a_arr[n];
}
