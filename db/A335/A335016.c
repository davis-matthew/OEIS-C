/*
 * A335016  Largest side lengths of equable Heronian triangles.
 * https://oeis.org/A335016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is the n-th largest side of an equable Heronian triangle.
 *            The values are obtained by exhaustive search over integer sides
 *            up to 200. There are exactly five such triangles, so the
 *            sequence is finite: 10, 13, 17, 20, 29
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* comparison function for qsort */
static int cmp_seq(const void *p1, const void *p2) {
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

/* Cached results of the search */
static seq_t *const_arr = NULL;
static size_t const_len = 0;
static int initialized = 0;

/* Compute the sequence values once */
static void build_sequence(void) {
    if (initialized) return;
    const uint64_t max_side = 200;   /* safe upper bound for the largest side (known to be 29) */
    size_t cap = 10;
    seq_t *arr = (seq_t *)malloc(cap * sizeof(seq_t));
    size_t len = 0;

    for (uint64_t a = 1; a <= max_side; ++a) {
        for (uint64_t b = a; b <= max_side; ++b) {
            for (uint64_t c = b; c <= max_side; ++c) {
                if (a + b <= c) continue;            /* triangle inequality */
                uint64_t P = a + b + c;
                if (P & 1) continue;                  /* perimeter must be even for integer semiperimeter */
                uint64_t s = P >> 1;
                uint64_t prod = s * (s - a) * (s - b) * (s - c);
                uint64_t Psq = P * P;
                if (prod == Psq) {                   /* area == perimeter and integer area */
                    if (len == cap) {
                        cap <<= 1;
                        arr = (seq_t *)realloc(arr, cap * sizeof(seq_t));
                    }
                    arr[len++] = (seq_t)c;            /* store the longest side */
                }
            }
        }
    }

    /* Sort the obtained longest sides */
    qsort(arr, len, sizeof(seq_t), cmp_seq);

    /* Store in static cache */
    const_arr = arr;
    const_len = len;
    initialized = 1;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                       /* offset starts at 1 */
    build_sequence();
    if ((idx_t)n > const_len) return 0;         /* undefined beyond the last term */
    return const_arr[(idx_t)n - 1];
}
