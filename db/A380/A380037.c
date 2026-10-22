/*
 * A380037  For n >= 1, if there exists an m < n such that a(m) = a(n), take
 *          the largest such m and set a(n+1) as 1 more than the number of
 *          positive terms between a(m+1) and a(n-1). Otherwise, a(n+1) = -1.
 *          Start with a(1) = 0.
 * https://oeis.org/A380037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 316, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A380037
 * Definition: a(1)=0.
 * For n≥1, let prev = a(n).  Find the largest m<n with a(m)=prev.
 * If such m exists, set a(n+1) = 1 + (number of positive terms a(m+1)…a(n-1)).
 * Otherwise, a(n+1) = -1.
 * This implementation memoizes the sequence up to the largest requested
 * index and uses a prefix array of positive counts to obtain the
 * required range sum in O(1). The search for m is a linear backward
 * scan, which is sufficiently fast for n≤10000. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static seq_t *seq = NULL;          /* sequence values, 1‑based */
static size_t *pos = NULL;         /* prefix counts of positive values */
static size_t seq_cap = 0;         /* current capacity of seq */
static size_t pos_cap = 0;         /* current capacity of pos */
static idx_t cur = 0;              /* highest index currently computed */

/* Initialise the data structures with the first term a(1)=0. */
static void init(void) {
    size_t init_cap = 16;               /* initial capacity, can grow */
    seq = (seq_t *)calloc(init_cap, sizeof(seq_t));
    pos = (size_t *)calloc(init_cap, sizeof(size_t));
    seq_cap = pos_cap = init_cap;
    seq[1] = (seq_t)0;
    pos[1] = 0;                         /* 0 positives so far */
    cur = 1;
}

/* Ensure that all terms up to index n are computed. */
static void ensure(idx_t n) {
    if (n <= cur) return;               /* already available */
    if (!seq) init();                   /* first call */

    /* Increase capacity if necessary */
    if ((size_t)n + 1 > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 2;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        pos = (size_t *)realloc(pos, new_cap * sizeof(size_t));
        seq_cap = pos_cap = new_cap;
    }

    /* Compute missing terms one by one */
    for (idx_t k = cur + 1; k <= n; ++k) {
        seq_t prev = seq[k - 1];
        idx_t m = 0;   /* 0 means not found */
        for (idx_t j = k - 2; j > 0; --j) {
            if (seq[j] == prev) { m = j; break; }
        }
        seq_t val;
        if (m == 0) {
            val = (seq_t)-1;            /* no previous occurrence */
        } else {
            /* count positives in a(m+1)…a(k-2) */
            size_t count = pos[k - 2] - pos[m];
            val = (seq_t)(count + 1);
        }
        seq[k] = val;
        pos[k] = pos[k - 1] + (val > 0 ? 1 : 0);
    }
    cur = n;
}

/* Public sequence function */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;      /* outside defined range */
    ensure(n);
    return seq[n];
}
