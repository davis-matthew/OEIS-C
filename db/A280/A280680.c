/*
 * A280680  The square roots of the radii (a, b, c) of three primitive
 *          mutually tangent circles all touching the same straight line, with
 *          a > b > c.
 * https://oeis.org/A280680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = the (n‑th) element of the flattened list of primitive
 *            triples (a,b,c) arising from three circles tangent to a line,
 *            sorted by the largest radius a
 * Range:     u8 none, u16 n <= 218, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"   /* seq_t gcd(seq_t a, seq_t b); */

/* Structure to hold a single triple (a,b,c). */
typedef struct {
    seq_t a;
    seq_t b;
    seq_t c;
} triple_t;

/* Comparator for qsort by 'a' ascending. */
static int cmp_triple_by_a(const void *p1, const void *p2)
{
    const triple_t *t1 = (const triple_t *)p1;
    const triple_t *t2 = (const triple_t *)p2;
    if (t1->a < t2->a) return -1;
    if (t1->a > t2->a) return 1;
    return 0;
}

/* Generate all primitive triples with radius indices up to MAX_B. */
/* The function returns a dynamically allocated array of triples
   and stores the number of triples in *cnt.  The caller is
   responsible for freeing the array. */
static triple_t *generate_triples(idx_t max_b, size_t *cnt)
{
    /* Rough upper bound on number of triples for allocation. */
    size_t est_cap = (size_t)(max_b * max_b / 8);
    triple_t *array = (triple_t *)malloc(est_cap * sizeof(triple_t));
    if (!array) return NULL;
    size_t capacity = est_cap;
    size_t count = 0;

    for (idx_t b = 2; b <= max_b; ++b) {
        idx_t c_start = (b >> 1) + 1;  /* c > b/2 to ensure a > b */
        for (idx_t c = c_start; c < b; ++c) {
            seq_t prod = (seq_t)b * (seq_t)c;
            seq_t denom = (seq_t)(b - c);
            if (prod % denom != 0) continue;
            seq_t a = prod / denom;
            if (a <= b) continue;      /* ensure a > b */
            seq_t g = gcd(a, (seq_t)b);
            g = gcd(g, (seq_t)c);
            if (g != 1) continue;      /* primitive triple */
            if (count == capacity) {
                capacity *= 2;
                triple_t *tmp = (triple_t *)realloc(array, capacity * sizeof(triple_t));
                if (!tmp) { free(array); return NULL; }
                array = tmp;
            }
            array[count].a = a;
            array[count].b = (seq_t)b;
            array[count].c = (seq_t)c;
            ++count;
        }
    }

    *cnt = count;
    return array;
}

seq_t a(idx_t n)
{
    /* Static state to compute the sequence only once. */
    static triple_t *triples = NULL;
    static size_t triple_cnt = 0;
    static seq_t *values = NULL;
    static size_t values_cnt = 0;

    if (!values) {
        /* Choose a sufficiently large upper bound for b to obtain >10 000 terms. */
        const idx_t MAX_B = 6000;
        size_t triples_requested = (size_t)((n + 2) / 3);  /* ceil(n/3) triples needed */

        /* Generate triples, maybe a bit more than needed. */
        triples = generate_triples(MAX_B, &triple_cnt);
        if (!triples) {
            /* In the unlikely event of OOM, return 0. */
            return (seq_t)0;
        }

        /* Sort triples by a ascending. */
        qsort(triples, triple_cnt, sizeof(triple_t), cmp_triple_by_a);

        /* Flatten into a single array of values (a,b,c). */
        values_cnt = triple_cnt * 3;
        values = (seq_t *)malloc(values_cnt * sizeof(seq_t));
        if (!values) {
            free(triples);
            return (seq_t)0;
        }
        for (size_t i = 0; i < triple_cnt; ++i) {
            values[3*i]     = triples[i].a;
            values[3*i + 1] = triples[i].b;
            values[3*i + 2] = triples[i].c;
        }
        free(triples);
    }

    /* Return the requested term.  n is 1‑based. */
    if (n == 0 || (idx_t)n > values_cnt) return (seq_t)0;
    return values[n-1];
}
