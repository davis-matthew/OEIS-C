/*
 * A353903  Number of partitions of n in which exactly one odd part is
 *          repeated and even parts are unrestricted.
 * https://oeis.org/A353903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m=1}^{⌊n/2⌋} b_{n-2m} · d_odd(m) where b_k = number of
 *            partitions of k with even parts unlimited and odd parts
 *            distinct, and d_odd(m) = number of odd divisors of m
 * Range:     u8 n <= 18, u16 n <= 47, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* global static arrays */
static seq_t *b = NULL;        /* b[k] */
static seq_t *dodd = NULL;    /* dodd[m] for m ≤ N/2 */
static seq_t *a_arr = NULL;   /* a_arr[n] */
static idx_t cached_n = 0;    /* maximum n computed so far */

/* recompute tables up to n (inclusive) */
static void compute_up_to(idx_t n)
{
    idx_t N = n;
    size_t size_b = (size_t)(N + 1);          /* indices 0..N */
    size_t size_dodd = (size_t)(N / 2 + 1);    /* indices 0..N/2 */

    /* reallocate arrays */
    b = (seq_t *)realloc(b, size_b * sizeof(seq_t));
    a_arr = (seq_t *)realloc(a_arr, size_b * sizeof(seq_t));
    dodd = (seq_t *)realloc(dodd, size_dodd * sizeof(seq_t));

    /* initialise */
    for (size_t i = 0; i < size_b; ++i) {
        b[i] = 0;
        a_arr[i] = 0;
    }
    b[0] = 1;      /* empty partition */

    for (size_t i = 0; i < size_dodd; ++i) dodd[i] = 0;

    /* DP for partitions with even unlimited and odd distinct */
    for (idx_t part = 1; part <= N; ++part) {
        if ((part & 1) == 0) {                 /* even part – unlimited */
            for (idx_t s = part; s <= N; ++s)
                b[s] += b[s - part];
        } else {                               /* odd part – at most one */
            for (idx_t s = N; s >= part; --s)
                b[s] += b[s - part];
        }
    }

    /* odd divisor counts up to N/2 */
    for (idx_t d = 1; d <= N / 2; d += 2) {    /* only odd d */
        for (idx_t m = d; m <= N / 2; m += d)
            dodd[m] += 1;
    }

    /* convolution to obtain a[n] */
    for (idx_t nidx = 1; nidx <= N; ++nidx) {
        seq_t sum = 0;
        idx_t maxm = nidx >> 1;               /* floor(n/2) */
        for (idx_t m = 1; m <= maxm; ++m) {
            sum += b[nidx - (m << 1)] * dodd[m];
        }
        a_arr[nidx] = sum;
    }

    cached_n = N;
}

seq_t a(idx_t n)
{
    if (n <= 0) return 0;              /* offset is 1, so n ≥ 1 */
    if (n > cached_n) compute_up_to(n);
    return a_arr[n];
}
