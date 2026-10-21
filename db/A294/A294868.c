/*
 * A294868  Solution of the complementary equation a(n) = 2*a(n-1) - a(n-2) +
 *          b(n-1) -2, where a(0) = 1, a(1) = 2, b(0) = 3, and (a(n)) and
 *          (b(n)) are increasing complementary sequences.
 * https://oeis.org/A294868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A294868 ----------------------------------------------------------
 *  Complementary sequences a(n) and b(n) defined by
 *
 *      a(0) = 1,  a(1) = 2,  b(0) = 3
 *      a(n) = 2*a(n-1) - a(n-2) + b(n-1) - 2   (n >= 2)
 *
 *  The sequences are complementary and increasing:
 *  every positive integer appears once in either a() or b().
 *  The function below builds the sequences incrementally and
 *  returns a(n) for any non‑negative index n.
 *
 *  The implementation keeps a local cache of already computed terms
 *  and a list of unused integer intervals that are needed to supply
 *  the “least new number” for b(n).  All operations are done in the
 *  integer type seq_t (which may be an arbitrary–precision class in
 *  C++ builds).  Memory allocation uses explicit casts so the code
 *  compiles as both C11 and C++17.
 *
 *  Complexity:      O(n) time and O(n) memory for computing a(0)…a(n).
 *  Correctness:     uses only integer arithmetic and the recurrence
 *  definition, so it is exact for all n that fit in seq_t.  The
 *  algorithm works for any order of calls, as the cached values
 *  are never invalidated.
 */

#include <stdlib.h>

/* --------------- internal types and storage -------------------------- */
static struct Interval {
    seq_t l, r;        /* ≤r is an unused integer interval          */
} *intervals = NULL;   /* dynamic array of intervals                */

static seq_t *a_seq = NULL;   /* a(0)...a(last computed)                  */
static seq_t *b_seq = NULL;   /* b(0)...b(last computed)                  */

static size_t a_cap = 0, b_cap = 0, intervals_cap = 0;
static size_t a_len = 0, b_len = 0, intervals_len = 0;
static size_t interval_start = 0;      /* index of next interval to use      */
static seq_t current_max = 0;          /* largest integer used so far        */
static int initialized = 0;            /* start-up flag                      */

/* --------------- main sequence function ----------------------------- */
seq_t a(idx_t n)
{
    /* --------- lazy static initialization --------------------------- */
    if (!initialized) {
        a_cap = 4;
        a_seq = (seq_t*)malloc(a_cap * sizeof(seq_t));
        b_cap = 4;
        b_seq = (seq_t*)malloc(b_cap * sizeof(seq_t));

        intervals_cap = 4;
        intervals = (struct Interval*)malloc(intervals_cap * sizeof(struct Interval));

        /* initial values */
        a_seq[0] = (seq_t)1;
        a_seq[1] = (seq_t)2;
        a_len  = 2;

        b_seq[0] = (seq_t)3;
        b_len  = 1;

        current_max = (seq_t)3;      /* maximum used so far                */
        intervals_len = 0;
        interval_start = 0;

        initialized = 1;
    }

    /* --------- trivial case ---------------------------------------- */
    if (n < a_len)
        return a_seq[n];

    /* --------- enlarge storage if necessary ------------------------ */
    if (a_cap < (size_t)(n + 1)) {
        size_t new_cap = a_cap ? a_cap : 4;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        a_seq = (seq_t*)realloc(a_seq, new_cap * sizeof(seq_t));
        a_cap = new_cap;
    }
    if (b_cap < (size_t)n) {           /* b holds one element less than a    */
        size_t new_cap = b_cap ? b_cap : 4;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        b_seq = (seq_t*)realloc(b_seq, new_cap * sizeof(seq_t));
        b_cap = new_cap;
    }

    /* --------- compute missing terms up to index n ----------------- */
    for (idx_t k = a_len; k <= n; ++k) {
        /* 1. choose the least unused number for b(k-1)  */
        seq_t b_next;
        if (interval_start < intervals_len) {
            b_next = intervals[interval_start].l;
            intervals[interval_start].l++;
            if (intervals[interval_start].l > intervals[interval_start].r)
                ++interval_start;
        } else {
            b_next = current_max + (seq_t)1;
            current_max = b_next;
        }
        b_seq[b_len++] = b_next;          /* store b(k-1) */

        /* 2. compute a(k) via the recurrence */
        seq_t a_next = (seq_t)2 * a_seq[k - 1] - a_seq[k - 2]
                       + b_seq[k - 1] - (seq_t)2;

        /* 3. record any new unused interval between old_max and a_next */
        if (a_next > current_max) {
            seq_t gap_start = current_max + (seq_t)1;
            seq_t gap_end   = a_next - (seq_t)1;
            if (gap_start <= gap_end) {
                if (intervals_len == intervals_cap) {
                    size_t new_cap = intervals_cap ? intervals_cap : 4;
                    while (new_cap <= intervals_len)
                        new_cap <<= 1;
                    intervals = (struct Interval*)realloc(intervals,
                            new_cap * sizeof(struct Interval));
                    intervals_cap = new_cap;
                }
                intervals[intervals_len].l = gap_start;
                intervals[intervals_len].r = gap_end;
                ++intervals_len;
            }
            current_max = a_next;
        }

        /* 4. store the newly computed a(k)                    */
        a_seq[a_len++] = a_next;
    }

    return a_seq[n];
}
