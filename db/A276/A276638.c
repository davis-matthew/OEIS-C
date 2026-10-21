/*
 * A276638  a(0)=0; thereafter a(n) is the number of matches between the first
 *          n terms circularly shifted by s and the reverse of the first n
 *          terms, maximized over s.
 * https://oeis.org/A276638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 3775, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A276638
 *  Formula: a(n) = max_{s=0..n-1} #{ j : a[(s+j)%n] == a[n-1-j] }
 *  with a(0) = 0.
 *  Implementation uses pair‑sum bookkeeping to obtain all match counts in
 *  O(∑_{i≤N} i) time (≈ O(N²) for N = 10 000).
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* types supplied by the build system */
extern seq_t a(idx_t n);

/* maximum n expected by the evaluation harness (≈10 000) */
#define MAX_N 10000
#define MAX_SUM (2 * MAX_N + 2)

static seq_t seq[MAX_N + 1];                 /* a(0) .. a(MAX_N) */
static idx_t seq_len = 0;                    /* number of computed terms */

/* unordered pair counts (including self‑pairs) indexed by p + q + 1 */
static unsigned int unordered_counts[MAX_SUM] = {0};

/* number of self‑pairs (p == q) indexed by 2 * p + 1 */
static unsigned int self_counts[MAX_SUM] = {0};

/* temporary array of per‑shift match counts */
static unsigned int shift_counts[MAX_N + 1] = {0};

/* ------------------------------------------------------------------ */
/* compute all terms up to a given n; called automatically by a(n)   */
static void ensure_seq_upto(idx_t n)
{
    /* initialise first term on first use */
    if (seq_len == 0) {
        seq[0] = 0;
        seq_len = 1;
        unordered_counts[1] = 1;   /* pair (0,0) */
        self_counts[1]    = 1;
    }

    /* extend sequence until it contains a(n) */
    while (seq_len <= n) {
        idx_t cur = seq_len;          /* current length before adding cur */
        size_t i;
        unsigned int max_match = 0;

        /* zero shift counts for the new length */
        for (i = 0; i < (size_t)cur; ++i)
            shift_counts[i] = 0;

        /* accumulate contributions from all unordered pairs */
        for (i = 1; i <= 2 * cur - 1; ++i) {
            unsigned int u = unordered_counts[i];
            if (u == 0)
                continue;
            /* matches contributed by sum i at any shift = i % cur */
            unsigned int m = 2 * u - self_counts[i];
            unsigned int rem = (unsigned int)(i % cur);
            shift_counts[rem] += m;
            if (shift_counts[rem] > max_match)
                max_match = shift_counts[rem];
        }

        /* store a(cur) */
        seq[cur] = (seq_t)max_match;

        /* update pair counts for the newly added element (index cur) */
        /* self‑pair of the new element */
        unsigned int k = 2 * cur + 1;
        unordered_counts[k]++;   /* unordered pair (cur,cur) */
        self_counts[k]++;        /* counts as a self pair */

        /* pairs with previously existing equal values */
        for (idx_t j = 0; j < cur; ++j) {
            if (seq[j] == seq[cur]) {
                k = j + cur + 1;   /* j + cur + 1 is the pair sum */
                unordered_counts[k]++;   /* unordered pair (j,cur) */
            }
        }

        ++seq_len;
    }
}

/* ------------------------------------------------------------------ */
/* public function required by the harness */
seq_t a(idx_t n)
{
    ensure_seq_upto(n);
    return seq[n];
}
