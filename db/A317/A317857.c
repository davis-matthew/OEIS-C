/*
 * A317857  Number of nX2 0..1 arrays with every element unequal to 1, 2 or 4
 *          horizontally, vertically or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A317857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - 2*a(n-3) - 2*a(n-4) + a(n-5) -
 *            4*a(n-6) + 4*a(n-7) - a(n-8) Initial values: a(1)=1, a(2)=3,
 *            a(3)=6, a(4)=10, a(5)=21, a(6)=46, a(7)=102, a(8)=220
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 57, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *memo = NULL;      /* memo[0] unused */
static size_t memo_cap = 0;     /* allocated size of memo array */
static idx_t memo_max = 0;      /* largest index that has been computed */

static const seq_t init_vals[9] = {
    0ULL,    /* unused */
    1ULL,    /* a(1) */
    3ULL,    /* a(2) */
    6ULL,    /* a(3) */
    10ULL,   /* a(4) */
    21ULL,   /* a(5) */
    46ULL,   /* a(6) */
    102ULL,  /* a(7) */
    220ULL   /* a(8) */
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* undefined for n<1 */

    /* First call: allocate enough space and initialise a(1)..a(8) */
    if (!memo) {
        size_t init_cap = (size_t)(n + 1);
        if (init_cap < 9) init_cap = 9;
        memo = (seq_t *)realloc(NULL, init_cap * sizeof(seq_t));
        if (!memo) abort();
        memo_cap = init_cap;
        for (idx_t i = 1; i <= 8; ++i) memo[(size_t)i] = init_vals[i];
        memo_max = 8;
    }

    /* If we need more capacity, grow the array */
    if ((size_t)n + 1 > memo_cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < memo_cap * 2) new_cap = memo_cap * 2;
        memo = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (!memo) abort();
        memo_cap = new_cap;
    }

    /* Return if already computed */
    if (n <= memo_max) return memo[(size_t)n];

    /* Compute terms up to n */
    for (idx_t i = memo_max + 1; i <= n; ++i) {
        seq_t v = memo[(size_t)(i - 1)];
        v += memo[(size_t)(i - 2)];
        v += memo[(size_t)(i - 2)];
        v += memo[(size_t)(i - 2)];
        v += memo[(size_t)(i - 2)];
        v -= memo[(size_t)(i - 3)];
        v -= memo[(size_t)(i - 3)];
        v -= memo[(size_t)(i - 4)];
        v -= memo[(size_t)(i - 4)];
        v += memo[(size_t)(i - 5)];
        v -= memo[(size_t)(i - 6)];
        v -= memo[(size_t)(i - 6)];
        v -= memo[(size_t)(i - 6)];
        v -= memo[(size_t)(i - 6)];
        v += memo[(size_t)(i - 7)];
        v += memo[(size_t)(i - 7)];
        v += memo[(size_t)(i - 7)];
        v += memo[(size_t)(i - 7)];
        v -= memo[(size_t)(i - 8)];
        memo[(size_t)i] = v;
    }

    memo_max = n;
    return memo[(size_t)n];
}
