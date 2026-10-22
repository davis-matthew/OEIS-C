/*
 * A384744  Numbers in which all substrings in base 16 are primes.
 * https://oeis.org/A384744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: exhaustive BFS enumeration of base‑16 numbers whose every
   contiguous hexadecimal substring is prime.  The function returns the
   nth term of OEIS A384744 (offset = 1).  It builds the complete finite
   list once and caches it for subsequent calls. */

#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime_big.h"  /* is_prime_big(seq_t) */

/* Node holds a candidate during breadth‑first construction. */
typedef struct Node {
    seq_t val;       /* current value (hexadecimal) */
    int   len;       /* number of hex digits */
    seq_t *subs;     /* substring values ending at each position */
} Node;

static seq_t  *seq_cache      = NULL;   /* sorted all terms */
static size_t seq_cache_len   = 0;
static bool   seq_cache_ready = false;

/* Append a node to a dynamic array. */
static void add_node(Node **arr, size_t *len, size_t *cap,
                     seq_t value, int sublen, seq_t *subs)
{
    if (*len == *cap) {
        *cap = (*cap == 0) ? 16 : (*cap << 1);
        *arr = (Node *)realloc(*arr, (*cap) * sizeof(Node));
    }
    (*arr)[*len].val  = value;
    (*arr)[*len].len  = sublen;
    (*arr)[*len].subs = subs;
    (*len)++;
}

/* Comparator for qsort on seq_t. */
static int cmp_seqt(const void *a, const void *b)
{
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

/* Generate the complete finite list of terms. */
static void generate_sequence(void)
{
    /* Single‑digit prime hex digits: 2,3,5,7,B(11),D(13). */
    const seq_t prime_digits[6] = { 2, 3, 5, 7, (seq_t)0xB, (seq_t)0xD };
    const int    num_digits     = 6;

    /* Dynamic arrays for nodes and results. */
    size_t node_cap = 0, node_len = 0;
    Node *nodes = NULL;

    size_t res_cap = 64, res_len = 0;
    seq_t *results = (seq_t *)malloc(res_cap * sizeof(seq_t));

    /* Initialize with 1‑digit primes. */
    for (int d = 0; d < num_digits; ++d) {
        seq_t digit = prime_digits[d];
        seq_t *sub  = (seq_t *)malloc(sizeof(seq_t));
        sub[0] = digit;
        add_node(&nodes, &node_len, &node_cap, digit, 1, sub);

        if (res_len == res_cap) {
            res_cap <<= 1;
            results = (seq_t *)realloc(results, res_cap * sizeof(seq_t));
        }
        results[res_len++] = digit;   /* each 1‑digit prime is a term */
    }

    /* BFS traversal: process nodes by increasing length. */
    for (size_t idx = 0; idx < node_len; ++idx) {
        Node cur = nodes[idx];

        /* Try appending every prime digit. */
        for (int d = 0; d < num_digits; ++d) {
            seq_t digit   = prime_digits[d];
            seq_t new_val = (cur.val << 4) | digit;

            seq_t *new_subs = (seq_t *)malloc((size_t)(cur.len + 1) * sizeof(seq_t));
            new_subs[cur.len] = digit;          /* substring of length 1 */

            bool ok = true;
            /* Build all substrings ending at the new digit and test. */
            for (int i = cur.len - 1; i >= 0; --i) {
                new_subs[i] = (cur.subs[i] << 4) | digit;
                if (!is_prime_big(new_subs[i])) {
                    ok = false;
                    break;
                }
            }

            if (ok) {
                add_node(&nodes, &node_len, &node_cap, new_val,
                         cur.len + 1, new_subs);

                if (res_len == res_cap) {
                    res_cap <<= 1;
                    results = (seq_t *)realloc(results, res_cap * sizeof(seq_t));
                }
                results[res_len++] = new_val;
            } else {
                free(new_subs);
            }
        }
        free(cur.subs);   /* finished with this node's substring array */
    }
    free(nodes);

    /* Sort the collected terms ascending. */
    qsort(results, res_len, sizeof(seq_t), cmp_seqt);

    seq_cache      = results;
    seq_cache_len  = res_len;
    seq_cache_ready = true;
}

/* Public API required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* offset is 1, but guard anyway */

    if (!seq_cache_ready) generate_sequence();

    if (n <= seq_cache_len) return seq_cache[n - 1];
    return 0;   /* sequence is finite; out‑of‑range queries return 0 */
}
