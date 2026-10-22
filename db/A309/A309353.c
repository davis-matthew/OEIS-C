/*
 * A309353  Let d(1), d(2), ..., d(q) be the q divisors of a number m. The
 *          sequence lists the numbers m such that {phi(d(1)), phi(d(2)), ...,
 *          phi(d(q))} is the set of the divisors of a number k, where phi is
 *          the Euler totient function.
 * https://oeis.org/A309353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 97, u64 n <= 300, u128 n <= 300,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A309353: a(n) = the n-th integer m such that the set {phi(d) : d|m}
 * is the set of all divisors of 2^k for some k.  An integer m satisfies
 * this iff it is of the form 2^a * Π p_i where the p_i are distinct
 * Fermat primes (3,5,17,257,65537) and a is at least the maximal gap
 * between consecutive subset sums of the corresponding exponent values
 * a_i = log₂(p_i−1).  The sequence is obtained by merging the
 * infinite geometric progressions base·2^a, a= min_a, min_a+1, … for
 * each admissible base.  See the analysis below for the derivation. */

#include <stdint.h>
#include <stdbool.h>

/* precomputed constants for the known Fermat primes */
#define NUM_FERMAT 5
static const uint64_t fermat_vals[NUM_FERMAT] = {3ULL,5ULL,17ULL,257ULL,65537ULL};
static const uint8_t fer_a[NUM_FERMAT] = {1,2,4,8,16};

struct heap_node {
    seq_t val;
    unsigned idx;     /* index of the base mask */
};

static inline void sift_up(struct heap_node *h, int sz, int i)
{
    for ( ; i > 0; i = (i - 1) / 2) {
        int p = (i - 1) / 2;
        if (h[p].val <= h[i].val) break;
        struct heap_node tmp = h[p];
        h[p] = h[i];
        h[i] = tmp;
    }
}

static inline void sift_down(struct heap_node *h, int sz, int i)
{
    for ( ; ; ) {
        int l = i * 2 + 1;
        if (l >= sz) break;
        int r = l + 1;
        int small = l;
        if (r < sz && h[r].val < h[l].val) small = r;
        if (h[small].val >= h[i].val) break;
        struct heap_node tmp = h[small];
        h[small] = h[i];
        h[i] = tmp;
        i = small;
    }
}

/* multiply a by two, returning the result */
static inline seq_t mul2(seq_t a)
{
    return a * (seq_t)2;
}

seq_t a(idx_t n)
{
    /* initialise the list of bases and minimal exponents once */
    static bool init = false;
    static seq_t base[1 << NUM_FERMAT];
    static uint8_t min_a[1 << NUM_FERMAT];
    if (!init) {
        const int MASKS = 1 << NUM_FERMAT;
        for (int m = 0; m < MASKS; ++m) {
            seq_t prod = (seq_t)1;
            /* base product */
            for (int i = 0; i < NUM_FERMAT; ++i)
                if (m & (1 << i))
                    prod = prod * (seq_t)fermat_vals[i];
            base[m] = prod;

            /* compute maximal gap between consecutive subset sums of a_i */
            uint8_t sum_lower = 0;
            uint8_t max_gap = 0;
            for (int i = 0; i < NUM_FERMAT; ++i) {
                if (m & (1 << i)) {
                    uint8_t gap = fer_a[i] - sum_lower;
                    if (gap > max_gap) max_gap = gap;
                    sum_lower += fer_a[i];
                }
            }
            /* minimal exponent of 2 needed */
            if (max_gap <= 1) min_a[m] = 0;
            else              min_a[m] = max_gap;
        }
        init = true;
    }

    /* heap with at most one element per mask */
    struct heap_node heap[1 << NUM_FERMAT];
    int hsize = 0;
    int MASKS = 1 << NUM_FERMAT;

    for (int i = 0; i < MASKS; ++i) {
        /* initial value for each mask = base * 2^min_a */
        seq_t val = base[i];
        for (uint8_t j = 0; j < min_a[i]; ++j)
            val = mul2(val);
        heap[hsize++] = (struct heap_node){val, (unsigned)i};
    }
    for (int i = hsize / 2 - 1; i >= 0; --i)
        sift_down(heap, hsize, i);

    seq_t result = 0;
    for (idx_t count = 1; count <= n; ++count) {
        /* extract minimum */
        struct heap_node top = heap[0];
        result = top.val;
        heap[0] = heap[--hsize];
        sift_down(heap, hsize, 0);

        /* insert next value from the same mask */
        struct heap_node nxt = {mul2(top.val), top.idx};
        heap[hsize++] = nxt;
        sift_up(heap, hsize, hsize - 1);
    }
    return result;
}
