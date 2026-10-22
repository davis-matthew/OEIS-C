/*
 * A331295  Number of values of k, 1 <= k <= n, with f(k) = f(n), where f(n) =
 *          [A001222(n), A061395(n)].
 * https://oeis.org/A331295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9960, u16 n <= 9960, u32 n <= 9960, u64 n <= 9960,
 *            u128 n <= 9960, big any n
 * Helpers:   bigomega, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A331295: Number of values of k, 1 <= k <= n, with f(k) = f(n),
   where f(n) = [A001222(n), A061395(n)].
   Computes a(n) by accumulating counts of seen pairs (d(n), bigomega(n)). */

/*
   The implementation keeps a cache of computed a(n) values, expanding it as needed.
   For each n it determines (omega, lpf) where
      omega   : number of prime factors of n counted with multiplicity     (Big Omega)
      lpf     : largest prime factor of n
   The first occurrence of a given pair receives count 1; subsequent
   occurrences increment the stored count.  The final count is returned as
   a(n).
   All arrays are grown in powers of two, and the allocation function
   handles both C11 and C++17 semantics.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/bigomega.h"
#include "../utils/largest_prime_factor.h"

typedef struct {
    seq_t omega;       /* Ω(n) */
    uint64_t lpf;      /* largest prime factor of n */
    seq_t count;       /* count of this pair seen so far */
} Node;

static seq_t *seq_vals   = NULL;   /* array of a(n) values */
static size_t seq_cap    = 0;      /* capacity of seq_vals */
static idx_t  max_computed = 0;    /* maximum n whose a(n) is computed */

static Node *nodes        = NULL;  /* array of unique pairs */
static size_t node_cap     = 0;    /* capacity of nodes array */
static size_t node_cnt     = 0;    /* number of unique pairs stored */

/* Expand a pointer 'p' to at least 'need' elements of size 'elem_size'. */
static void *grow(void *p, size_t *cap, size_t elem_size, size_t need)
{
    if (*cap >= need) return p;
    size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
    while (new_cap < need) new_cap <<= 1;
    p = (void *)realloc(p, new_cap * elem_size);
    *cap = new_cap;
    return p;
}

/* External function required by harness */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1 – not expected in normal use */

    /* Expand storage if we need to compute up to n */
    if (n > max_computed) {
        /* Ensure we have room for a(n) values */
        seq_vals = (seq_t *)grow(seq_vals, &seq_cap, sizeof(seq_t), n);
        /* Ensure we have room for all distinct pairs seen so far when
           processing new numbers (max possible is n, but we grow
           dynamically). */
        nodes = (Node *)grow(nodes, &node_cap, sizeof(Node), node_cnt + (size_t)(n - max_computed));

        /* Compute values from max_computed+1 up to n */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t   omega = bigomega(i);                     /* Ω(i) */
            uint64_t lpf  = (i == 1) ? 1ULL : largest_prime_factor((uint64_t)i);

            /* Search for existing pair */
            size_t j;
            for (j = 0; j < node_cnt; ++j) {
                if (nodes[j].omega == omega && nodes[j].lpf == lpf)
                    break;
            }

            seq_t val;
            if (j == node_cnt) {            /* new pair encountered */
                nodes[node_cnt].omega = omega;
                nodes[node_cnt].lpf   = lpf;
                nodes[node_cnt].count = (seq_t)1;
                val = (seq_t)1;
                ++node_cnt;
            } else {                        /* existing pair */
                nodes[j].count++;
                val = nodes[j].count;
            }

            seq_vals[max_computed] = val;
            ++max_computed;
        }
    }

    return seq_vals[n - 1];
}
