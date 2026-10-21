/*
 * A295617  Solution of the complementary equation a(n) = 2*a(n-1) - a(n-3) +
 *          b(n-2), where a(0) = 1, a(1) = 3, a(2) = 5, b(0) = 2, b(1) = 4,
 *          b(2) = 6, and (a(n)) and (b(n)) are increasing complementary
 *          sequences.
 * https://oeis.org/A295617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-3) + b(n-2); Initial values: a(0)=1,
 *            a(1)=3, a(2)=5, b(0)=2, b(1)=4, b(2)=6. The sequences a() and
 *            b() are complementary increasing sequences that satisfy the
 *            above recurrence. This implementation builds the sequences on
 *            demand using the recurrence for a and generates the re...
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *a_vals = NULL;
static size_t a_cap = 0;
static size_t a_len = 0;

static seq_t *b_vals = NULL;
static size_t b_cap = 0;
static size_t b_len = 0;

static seq_t last_b_val = 0;   /* last b value that has been generated */

/* Grow a dynamic array of seq_t to accommodate at least 'need' elements */
static void ensure_capacity_seq_t(seq_t **arr, size_t *cap, size_t need)
{
    if (need > *cap) {
        size_t new_cap = (*cap == 0) ? 4 : *cap;
        while (new_cap < need) new_cap <<= 1;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* Initialise static storage with the first few terms of a[] and b[] */
static void init_tables(void)
{
    if (a_len == 0) {
        ensure_capacity_seq_t(&a_vals, &a_cap, 3);
        a_vals[0] = 1;
        a_vals[1] = 3;
        a_vals[2] = 5;
        a_len = 3;

        ensure_capacity_seq_t(&b_vals, &b_cap, 3);
        b_vals[0] = 2;
        b_vals[1] = 4;
        b_vals[2] = 6;
        b_len = 3;

        last_b_val = 6;
    }
}

/* Binary search for a  value that is already present in a_vals[]. */
static int contains_a(seq_t val)
{
    size_t lo = 0, hi = a_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (a_vals[mid] == val) return 1;
        else if (a_vals[mid] < val) lo = mid + 1;
        else hi = mid;
    }
    return 0;
}

/* Generate all a[] up to index n (inclusive) and any necessary b[] values. */
static void ensure_up_to(idx_t n)
{
    init_tables();
    while (a_len <= (size_t)n) {
        size_t k = a_len;                     /* index we are about to compute */
        /* Ensure we have b[k-2] ready for the recurrence */
        size_t needed_b = (k >= 2) ? k - 2 : 0;
        while (b_len <= needed_b) {          /* generate next b value(s) as needed */
            seq_t cand = last_b_val + 1;
            while (contains_a(cand)) cand = cand + 1;   /* skip numbers already in a */
            ensure_capacity_seq_t(&b_vals, &b_cap, b_len + 1);
            b_vals[b_len++] = cand;
            last_b_val = cand;
        }
        /* Compute a[k] using the recurrence */
        seq_t a_k = 2 * a_vals[k - 1] - a_vals[k - 3] + b_vals[k - 2];
        ensure_capacity_seq_t(&a_vals, &a_cap, a_len + 1);
        a_vals[a_len++] = a_k;
    }
}

/* Public function required by the exercise. */
seq_t a(idx_t n)
{
    ensure_up_to(n);
    return a_vals[n];
}
