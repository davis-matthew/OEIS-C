/*
 * A391112  a(n) is the number of Heronian triangles with semiperimeter n that
 *          can be split into two Pythagorean triangles.
 * https://oeis.org/A391112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:  count Heronian triangles with semiperimeter n that can be split into two
 * Pythagorean triangles.  The routine builds all Pythagorean triples with sides
 * ≤ 2n, stores for each possible perpendicular side the collection of
 * complementary triples, and then tests all pairs of such triples for
 * triangle and integer‑area conditions.  Results are cached in a static array
 * so subsequent calls are O(1).  The algorithm follows the definition
 * in the OEIS entry directly.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* A minimal pair consisting of the other leg and the hypotenuse */
typedef struct {
    int other;   /* the leg of the triangle other than the common side */
    int hyp;     /* the hypotenuse of the right triangle */
} pair_t;

/* Dense array of pairs for a specific side length */
typedef struct {
    int size;
    int cap;
    pair_t *data;
} list_t;

/* Cache of previously computed sequence values */
static seq_t *cached_values = NULL;
static idx_t cache_max_index = 0;

/* Append a new pair to a list, reallocating if necessary. */
static void append_pair(list_t *lst, int other, int hyp)
{
    if (lst->cap == 0) {
        lst->cap = 4;
        lst->data = (pair_t *)malloc(lst->cap * sizeof(pair_t));
    } else if (lst->size == lst->cap) {
        lst->cap <<= 1;
        lst->data = (pair_t *)realloc(lst->data, lst->cap * sizeof(pair_t));
    }
    lst->data[lst->size].other = other;
    lst->data[lst->size].hyp  = hyp;
    lst->size++;
}

/* Recompute the cache up to a new maximum index. */
static void recompute_cache(idx_t max_n)
{
    /* Allocate new cache array */
    if (cached_values != NULL) {
        free(cached_values);
    }
    cached_values = (seq_t *)calloc(max_n + 1, sizeof(seq_t));
    if (cached_values == NULL) {
        return;                 /* allocation failed */
    }
    cache_max_index = max_n;

    /* Maximum side length that can appear in a triangle with semiperimeter ≤ max_n */
    idx_t L = max_n * 2;

    /* Prepare a list for each possible common side length */
    list_t *lists = (list_t *)calloc(L + 1, sizeof(list_t));
    if (lists == NULL) {
        free(cached_values);
        cached_values = NULL;
        cache_max_index = 0;
        return;
    }

    /* Generate all primitive Pythagorean triples and their multiples. */
    for (int m = 2; (idx_t)m * m <= L; ++m) {
        for (int n = 1; n < m; ++n) {
            if (((m - n) & 1) == 0) {
                continue;                         /* m and n must have opposite parity */
            }
            if (gcd((seq_t)m, (seq_t)n) != (seq_t)1) {
                continue;                         /* must be coprime */
            }
            int a = m * m - n * n;               /* one leg */
            int b = 2 * m * n;                   /* other leg */
            int c = m * m + n * n;               /* hypotenuse */

            if (c > L) {
                continue;                         /* any multiple will be too large */
            }

            int kmax = L / a;
            if (L / b < kmax) kmax = L / b;

            for (int k = 1; k <= kmax; ++k) {
                int A = a * k;
                int B = b * k;
                int C = c * k;

                if (A <= L) {
                    append_pair(&lists[A], B, C);
                }
                if (B <= L) {
                    append_pair(&lists[B], A, C);
                }
            }
        }
    }

    /* Count valid Heronian triangles for each semiperimeter. */
    for (int h = 1; h <= L; ++h) {
        list_t *lst = &lists[h];
        for (int i = 0; i < lst->size; ++i) {
            int u = lst->data[i].other;
            int a = lst->data[i].hyp;
            for (int j = i; j < lst->size; ++j) {
                int v = lst->data[j].other;
                int b = lst->data[j].hyp;

                int c = u + v;
                int sum = a + b + c;
                if (sum & 1) {                     /* semiperimeter would be non‑integer */
                    continue;
                }
                idx_t s = sum >> 1;
                if (s == 0 || s > max_n) {
                    continue;                   /* outside cache bounds */
                }

                /* Integer area condition: h*c must be even */
                if (((uint64_t)h * (uint64_t)c) & 1ULL) {
                    continue;
                }

                /* Triangle inequalities */
                if (a + b <= c) continue;
                if (a + c <= b) continue;
                if (b + c <= a) continue;

                /* Valid triangle found – increment its count */
                cached_values[s] += (seq_t)1;
            }
        }
    }

    /* Free temporary list storage */
    for (int h = 1; h <= L; ++h) {
        if (lists[h].data != NULL) {
            free(lists[h].data);
        }
    }
    free(lists);
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }
    if (n > cache_max_index) {
        recompute_cache(n);
    }
    return cached_values[n];
}
