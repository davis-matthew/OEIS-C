/*
 * A360711  Partial sums of A360710.
 * https://oeis.org/A360711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = Sum_{k=1..n} A360710(k), where A360710 is multiplicative
 *            with a(p^k) = +-1 chosen to minimize |Sum_{m<=p^k} a(m)|, ties
 *            -> a(p^k-1); computed by a sieve
 * Range:     i8 n <= 6085, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 94 terms through n = 9787 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static int *p360711_pref = 0;
static unsigned long long p360711_done = 0;

static void p360711_build(unsigned long long N)
{
    uint32_t *spf = (uint32_t *)calloc((size_t)N + 1, sizeof(uint32_t));
    signed char *sg = (signed char *)calloc((size_t)N + 1, 1);
    int *pf = (int *)calloc((size_t)N + 1, sizeof(int));
    unsigned long long i, j;
    for (i = 2; i <= N; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= N; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    sg[1] = 1;
    pf[1] = 1;
    for (i = 2; i <= N; i++) {
        unsigned long long p = spf[i], q = 1, t = i;
        while (t % p == 0) { t /= p; q *= p; }
        if (t == 1) {
            int s = pf[i - 1];
            int ap = s + 1; if (ap < 0) ap = -ap;
            int am = s - 1; if (am < 0) am = -am;
            if (ap < am) sg[i] = 1;
            else if (am < ap) sg[i] = -1;
            else sg[i] = sg[i - 1];
        } else {
            sg[i] = (signed char)(sg[q] * sg[t]);
        }
        pf[i] = pf[i - 1] + sg[i];
    }
    free(spf);
    free(sg);
    if (p360711_pref) free(p360711_pref);
    p360711_pref = pf;
    p360711_done = N;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n > p360711_done) {
        unsigned long long N = (unsigned long long)n;
        if (N < 2 * p360711_done) N = 2 * p360711_done;
        if (N < 1024) N = 1024;
        p360711_build(N);
    }
    return (seq_t)p360711_pref[n];
}
