/*
 * A272063  a(n) = largest k such that A004431(n) +/- k are both positive
 *          squares.
 * https://oeis.org/A272063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest k such that A004431(n) +/- k are both positive
 *            squares. A004431(n) is the nth integer that can be written as
 *            (x^2 + y^2)/2 with x>y>0 and x,y having the same parity. The
 *            following computes a(n) by generating all pairs (x,y), forming
 *            t=(x^2+y^2)/2 and k=(x^2-y^2)/2, keeping the larg...
 * Range:     u8 none, u16 none, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Pair of a value t and its corresponding maximum k. */
typedef struct {
    seq_t t;
    seq_t k;
} Pair;

/* Comparator for qsort: order by t, then by k if needed. */
static int pair_cmp(const void *p1, const void *p2)
{
    const Pair *a = (const Pair *)p1;
    const Pair *b = (const Pair *)p2;
    if (a->t < b->t) return -1;
    if (a->t > b->t) return 1;
    return 0;
}

/* Static storage for computed a(n) values.  a_values[1] is a(1). */
static seq_t *a_values = NULL;
static size_t a_len = 0;        /* actual number of values stored */

/* Compute a(n) values up to at least max_n entries. */
static void compute_sequence(idx_t max_n)
{
    /* The following fixed limit is enough to generate >10 000 terms. */
    const idx_t X_MAX = 1000;

    /* Rough upper bound on number of pairs (x,y) with 1<=y<x<=X_MAX. */
    unsigned long long pair_est = (unsigned long long)X_MAX * (X_MAX - 1) / 2 + 10;
    size_t max_pairs = (size_t)pair_est;

    Pair *pairs = (Pair *)malloc(max_pairs * sizeof(Pair));
    size_t pair_cnt = 0;

    for (idx_t x = 2; x <= X_MAX; ++x) {
        seq_t x2 = (seq_t)x * (seq_t)x;
        for (idx_t y = 1; y < x; ++y) {
            /* same parity: (x-y) even */
            if (((x - y) & 1) != 0) continue;
            seq_t y2 = (seq_t)y * (seq_t)y;
            seq_t sum = x2 + y2;                /* even for same parity */
            seq_t t   = sum >> 1;               /* (x^2 + y^2)/2  */
            if (t == 0) continue;
            seq_t diff = x2 - y2;               /* >=1 since x>y */
            seq_t k   = diff >> 1;              /* (x^2 - y^2)/2  */
            pairs[pair_cnt].t = t;
            pairs[pair_cnt].k = k;
            ++pair_cnt;
        }
    }

    /* Sort pairs by t. */
    qsort(pairs, pair_cnt, sizeof(Pair), pair_cmp);

    /* Deduplicate: keep for each distinct t the maximum k. */
    a_len = 0;
    for (size_t i = 0; i < pair_cnt; ++i) {
        if (i == 0 || pairs[i].t != pairs[i-1].t) {
            /* new distinct t */
            pairs[a_len] = pairs[i];
            ++a_len;
        } else if (pairs[i].k > pairs[a_len-1].k) {
            /* same t, keep larger k */
            pairs[a_len-1].k = pairs[i].k;
        }
    }

    /* Allocate array for a(n).  Store 1‑based indexes. */
    a_values = (seq_t *)malloc((a_len + 1) * sizeof(seq_t));
    for (size_t idx = 1; idx <= a_len; ++idx) {
        a_values[idx] = pairs[idx - 1].k;
    }

    free(pairs);
}

/* Function required by the build. */
seq_t a(idx_t n)
{
    /* Compute the sequence once, if not yet done. */
    if (a_len == 0) {
        /* The first 10 000 terms are produced by X_MAX=1000. */
        compute_sequence(n);
    }

    /* If n exceeds precomputed range, extend the table. */
    if ((size_t)n > a_len) {
        compute_sequence(n);
    }

    return a_values[(size_t)n];
}
