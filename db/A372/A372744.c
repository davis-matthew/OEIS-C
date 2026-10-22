/*
 * A372744  If the n-th 3-smooth number, A003586(n), equals 2^i * 3^j for some
 *          i, j >= 0, then the a(n)-th 3-smooth number, A003586(a(n)), equals
 *          2^j * 3^i.
 * https://oeis.org/A372744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 117, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Function for OEIS A372744:
 *  a(n) is the index of the number obtained by swapping the exponents of 2
 *  and 3 in the n‑th 3‑smooth number (i.e. the sequence is the
 *  self‑inverse permutation induced by the map 2^i 3^j ↦ 2^j 3^i). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --- CONSTANTS ------------------------------------------------------- */
#define MAX_E 2000  /* max exponent for 2 and 3 that covers n up to >10000 */

/* logarithm base 2 of 3 (precise to 64 bits) */
static const long double LOG2_3 = 1.584962500721156181453535744850582L;

/* --- TYPES ------------------------------------------------------------- */
typedef struct {
    uint16_t i;
    uint16_t j;
} pair_t;

/* --- STATIC STATE ----------------------------------------------------- */
static pair_t *sorted_pairs = NULL; /* sorted list of all exponent pairs */
static size_t   sorted_count = 0;    /* number of pairs */

/* --- HELPERS ----------------------------------------------------------- */
static int cmp_pair(const void *a, const void *b)
{
    const pair_t *p = (const pair_t *)a;
    const pair_t *q = (const pair_t *)b;
    long double va = (long double)p->i + (long double)p->j * LOG2_3;
    long double vb = (long double)q->i + (long double)q->j * LOG2_3;
    if (va < vb) return -1;
    if (va > vb) return  1;
    return 0;
}

/* initialise the sorted array once */
static void init_once(void)
{
    if (sorted_pairs != NULL)
        return; /* already initialised */

    sorted_count = (size_t)(MAX_E + 1) * (size_t)(MAX_E + 1);
    sorted_pairs = (pair_t *)malloc(sorted_count * sizeof(pair_t));
    if (sorted_pairs == NULL)
        abort(); /* allocation failed */

    size_t k = 0;
    for (uint16_t i = 0; i <= MAX_E; ++i)
        for (uint16_t j = 0; j <= MAX_E; ++j)
            sorted_pairs[k++] = (pair_t){i, j};

    qsort(sorted_pairs, sorted_count, sizeof(pair_t), cmp_pair);
}

/* binary search for the index of (i,j) in sorted_pairs */
static size_t find_index(uint16_t i, uint16_t j)
{
    size_t lo = 0;
    size_t hi = sorted_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        pair_t cur = sorted_pairs[mid];
        if (cur.i == i && cur.j == j)
            return mid;

        long double tm = (long double)i + (long double)j * LOG2_3;
        long double cm = (long double)cur.i + (long double)cur.j * LOG2_3;
        if (tm < cm)
            hi = mid;
        else
            lo = mid + 1;
    }
    /* The pair must exist; abort on failure. */
    abort();
    return 0; /* unreachable */
}

/* --- SEQUENCE FUNCTION ----------------------------------------------- */
seq_t a(idx_t n)
{
    init_once();                         /* ensure data is ready */

    /* n is 1‑based; indices in array are 0‑based */
    size_t idx = (size_t)(n - 1);
    if (idx >= sorted_count)
        abort();                         /* n too large for pre‑generated set */

    pair_t p = sorted_pairs[idx];        /* (i,j) of n‑th 3‑smooth number */
    size_t swapped_idx = find_index(p.j, p.i);   /* swap exponents */

    return (seq_t)(swapped_idx + 1);     /* back to 1‑based index */
}
