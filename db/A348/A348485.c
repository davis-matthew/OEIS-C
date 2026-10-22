/*
 * A348485  Lexicographically earliest sequence of positive integers in which,
 *          for all positive k, there are exactly k contiguous pairs whose
 *          product is k, and a(k) * a(k+1) <> a(k+1) * a(k+2).
 * https://oeis.org/A348485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Deterministic greedy construction of the lexicographically earliest
 * sequence a(n) such that for every k≥1 there are exactly k contiguous
 * pairs whose product equals k, and no three successive terms satisfy
 * a(i)=a(i+2).  The algorithm keeps the counts of all products that
 * have already appeared, and at each step chooses the smallest
 * candidate that does not exceed the allowed count for its product
 * and does not reuse a term two positions earlier.
 */
#include <stdlib.h>
#include <stdint.h>

/* contiguous pairs product data */
typedef struct {
    seq_t prod;   /* product of two consecutive terms */
    int  count;   /* how many times this product has already occurred */
} Pair;

/* global static data used for memoisation */
static seq_t *seq      = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;

static Pair *pairs     = NULL;
static size_t pairs_len  = 0;
static size_t pairs_cap  = 0;

/* ensure seq has capacity for at least `need` elements */
static void seq_ensure(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* ensure pairs has capacity for at least `need` entries */
static void pairs_ensure(size_t need)
{
    if (need <= pairs_cap) return;
    size_t new_cap = pairs_cap ? pairs_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    pairs = (Pair *)realloc(pairs, new_cap * sizeof(Pair));
    pairs_cap = new_cap;
}

/* linear search for a product, returns index or SIZE_MAX */
static size_t find_pair(seq_t prod)
{
    for (size_t i = 0; i < pairs_len; ++i) {
        if (pairs[i].prod == prod) return i;
    }
    return SIZE_MAX;
}

/* get current count of product `prod` */
static int get_pair_count(seq_t prod)
{
    size_t idx = find_pair(prod);
    return (idx == SIZE_MAX) ? 0 : pairs[idx].count;
}

/* increment the count for product `prod` */
static void inc_pair(seq_t prod)
{
    size_t idx = find_pair(prod);
    if (idx != SIZE_MAX) {
        pairs[idx].count++;
    } else {
        pairs_ensure(pairs_len + 1);
        pairs[pairs_len].prod   = prod;
        pairs[pairs_len].count  = 1;
        pairs_len++;
    }
}

/* public function to obtain the nth term (1‑based) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* out of range – undefined behaviour otherwise */

    /* generate term n if not already present */
    if (seq_len < (size_t)n) {
        seq_ensure((size_t)n);
        while (seq_len < (size_t)n) {
            if (seq_len == 0) {        /* a(1) = 1 */
                seq[0] = (seq_t)1;
                seq_len = 1;
                continue;
            }

            seq_t prev = seq[seq_len - 1];
            seq_t cand = (seq_t)1;

            for (;;) {
                seq_t prod = prev * cand;
                int used   = get_pair_count(prod);

                /* a product may appear at most `prod` times */
                if ((seq_t)used < prod) {
                    /* avoid a(i)=a(i+2) */
                    if (seq_len >= 2 && seq[seq_len - 2] == cand) {
                        cand++;
                        continue;
                    }

                    seq[seq_len] = cand;
                    inc_pair(prod);
                    break;
                }
                cand++;
            }
            seq_len++;
        }
    }
    return seq[(size_t)n - 1];
}
