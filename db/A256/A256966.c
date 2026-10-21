/*
 * A256966  Partial sums of A072649.
 * https://oeis.org/A256966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{n} A072649(k) The sequence A072649 is
 *            1,2,3,3,4,4,4,5,5,5,5,5,6,6,6,6,6,6,6,6,7,…, i.e. each integer m
 *            appears Fib(m) times (Fib(1)=1,Fib(2)=1,…). With this block‐size
 *            interpretation a(n) = Σ_{i=1}^{m-1} i·Fib(i) + m·( n –
 *            Σ_{i=1}^{m-1} Fib(i) ) where m is the smallest index wit...
 * Range:     u8 n <= 41, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static tables of Fibonacci numbers and running sums. */
    static seq_t *fib = NULL;          /* fib[i] = Fib(i), fib[0] unused */
    static seq_t *cnt = NULL;          /* cnt[i]  = Σ_{j=1}^i Fib(j) */
    static seq_t *weight = NULL;       /* weight[i] = Σ_{j=1}^i j·Fib(j) */
    static size_t cap = 0;             /* current allocated capacity */

    /* Ensure the tables are initialized (only once). */
    if (cap == 0) {
        cap = 16;                     /* starting capacity, enough for n≈1e7 */
        fib    = (seq_t*)malloc(cap * sizeof(seq_t));
        cnt    = (seq_t*)malloc(cap * sizeof(seq_t));
        weight = (seq_t*)malloc(cap * sizeof(seq_t));

        fib[0]      = (seq_t)0;            /* unused */
        fib[1]      = (seq_t)1;
        fib[2]      = (seq_t)1;
        cnt[0]      = (seq_t)0;
        cnt[1]      = fib[1];
        cnt[2]      = cnt[1] + fib[2];
        weight[0]   = (seq_t)0;
        weight[1]   = (seq_t)1 * fib[1];
        weight[2]   = weight[1] + (seq_t)2 * fib[2];
    }

    /* Return 0 for indices below the offset; it does not matter. */
    if (n < 1) return (seq_t)0;

    /* Grow the tables if needed to cover the requested n. */
    seq_t N = (seq_t)n;
    size_t i = 1;
    while (cnt[i] < N) {
        /* Grow capacity if necessary */
        if (i + 1 >= cap) {
            size_t newcap = cap * 2;
            seq_t *nfib    = (seq_t*)malloc(newcap * sizeof(seq_t));
            seq_t *ncnt    = (seq_t*)malloc(newcap * sizeof(seq_t));
            seq_t *nweight = (seq_t*)malloc(newcap * sizeof(seq_t));
            for (size_t k = 0; k < cap; ++k) {
                nfib[k]    = fib[k];
                ncnt[k]    = cnt[k];
                nweight[k] = weight[k];
            }
            free(fib);
            free(cnt);
            free(weight);
            fib    = nfib;
            cnt    = ncnt;
            weight = nweight;
            cap    = newcap;
        }

        /* Compute next Fibonacci number and the corresponding sums. */
        ++i;   /* Now i points to the index to compute */
        fib[i]      = fib[i - 1] + fib[i - 2];
        cnt[i]      = cnt[i - 1] + fib[i];
        weight[i]   = weight[i - 1] + (seq_t)i * fib[i];
    }

    /* i now satisfies cnt[i] >= N, so (i-1) is the last fully completed block. */
    size_t prev = i - 1;
    seq_t remaining = N - cnt[prev];           /* how many positions into block i */
    return weight[prev] + (seq_t)i * remaining;
}
