/*
 * A287868  Positions of 2's in A103270.
 * https://oeis.org/A287868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th 2 in A103270, where A103270(m) =
 *            (prime(m)+prime(m+k)) mod 4, k=(prime(m+1)-prime(m))/2; found by
 *            scanning m = 2,3,... with a sieve
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A287868_pr = NULL;
static size_t A287868_cnt = 0;
static uint64_t A287868_lim = 0;

static void A287868_ensure(uint64_t idx) {
    while (A287868_cnt < idx) {
        uint64_t lim = A287868_lim ? A287868_lim * 2 : 1024;
        char *s = (char *)calloc((size_t)lim + 1, 1);
        if (!s) abort();
        size_t c = 0;
        for (uint64_t i = 2; i <= lim; i++) {
            if (!s[i]) {
                c++;
                for (uint64_t j = i * i; j <= lim; j += i) s[j] = 1;
            }
        }
        uint64_t *np = (uint64_t *)realloc(A287868_pr, (c + 1) * sizeof(uint64_t));
        if (!np) abort();
        A287868_pr = np;
        size_t t = 0;
        for (uint64_t i = 2; i <= lim; i++)
            if (!s[i]) A287868_pr[t++] = i;
        A287868_cnt = t;
        A287868_lim = lim;
        free(s);
    }
}

static uint64_t *A287868_found = NULL;
static size_t A287868_fcnt = 0, A287868_fcap = 0;
static uint64_t A287868_next = 2;

seq_t a(idx_t n)
{
    while (A287868_fcnt < n) {
        uint64_t m = A287868_next++;
        A287868_ensure(m + 1);
        uint64_t p1 = A287868_pr[m - 1];
        uint64_t p2 = A287868_pr[m];
        uint64_t k = (p2 - p1) / 2;
        A287868_ensure(m + k);
        uint64_t p3 = A287868_pr[m + k - 1];
        p1 = A287868_pr[m - 1];
        if (((p1 + p3) & 3) == 2) {
            if (A287868_fcnt == A287868_fcap) {
                size_t nc = A287868_fcap ? A287868_fcap * 2 : 64;
                uint64_t *t = (uint64_t *)realloc(A287868_found, nc * sizeof(uint64_t));
                if (!t) abort();
                A287868_found = t;
                A287868_fcap = nc;
            }
            A287868_found[A287868_fcnt++] = m;
        }
    }
    return (seq_t)A287868_found[n - 1];
}
