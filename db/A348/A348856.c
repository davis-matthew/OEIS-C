/*
 * A348856  a(n) = Sum_{d|n} (Stirling2(n,d) mod 2).
 * https://oeis.org/A348856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = SUM_{d|n} (Stirling2(n,d) mod 2)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint8_t *S_data = NULL;   /* flat table of Stirling2(n,k) mod 2 */
static idx_t Nmax = 0;           /* maximum n for which table is ready */

/* Grow the table up to at least N */
static void ensure(idx_t N) {
    if (N <= Nmax)
        return;
    /* size = (N+1)(N+2)/2 bytes, one byte per entry of the triangle */
    size_t new_size = (size_t)(N + 1) * (N + 2) / 2;
    uint8_t *new_data = (uint8_t *)realloc(S_data, new_size);
    if (!new_data)            /* fatal: out of memory */
        exit(1);
    S_data = new_data;

    /* initialize new rows */
    for (idx_t n = Nmax + 1; n <= N; ++n) {
        size_t base = (size_t)n * (n + 1) / 2;          /* S[n][0] is here */
        /* base + k = index of S[n][k] */
        S_data[base + 0] = 0;           /* S(n,0)=0 for n>0 */
        S_data[base + n] = 1;           /* S(n,n)=1 */
        if (n == 0) {                  /* base case n==0 */
            S_data[0] = 1;              /* S(0,0)=1 */
            continue;
        }
        size_t prev_base = (size_t)(n - 1) * n / 2; /* S[n‑1] row starts here */
        for (idx_t k = 1; k < n; ++k) {
            /* S(n,k)=k*S(n-1,k)+S(n-1,k-1)  (mod 2) */
            uint8_t v = ((k & 1) && S_data[prev_base + k]) ^ S_data[prev_base + k - 1];
            S_data[base + k] = v;
        }
    }
    Nmax = N;
}

/* compute a(n) */
seq_t a(idx_t n) {
    if (n == 0)                 /* undefined for n=0, return 0 */
        return 0;
    ensure(n);

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            size_t idx = (size_t)n * (n + 1) / 2 + d;
            if (S_data[idx])
                sum++;
            idx_t d2 = n / d;
            if (d2 != d) {
                idx = (size_t)n * (n + 1) / 2 + d2;
                if (S_data[idx])
                    sum++;
            }
        }
    }
    return sum;
}
