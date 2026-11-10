/*
 * A305797  Dirichlet convolution of A078898 with A078899.
 * https://oeis.org/A305797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A078898(d)*A078899(n/d), computed by sieving
 *            smallest/greatest prime factors
 * Range:     u8 n <= 119, u16 n <= 14399, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 111 terms through n = 9938 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

static uint64_t *F_arr = NULL; /* A078898 values, index 1..cap */
static uint64_t *G_arr = NULL; /* A078899 values, index 1..cap */
static uint64_t cap_n = 0;

static void build_tables(uint64_t limit)
{
    uint64_t i, m;
    uint64_t *spf, *gpf, *cnt;
    free(F_arr);
    free(G_arr);
    F_arr = (uint64_t *)calloc((size_t)limit + 1, sizeof(uint64_t));
    G_arr = (uint64_t *)calloc((size_t)limit + 1, sizeof(uint64_t));
    spf = (uint64_t *)calloc((size_t)limit + 1, sizeof(uint64_t));
    gpf = (uint64_t *)calloc((size_t)limit + 1, sizeof(uint64_t));
    cnt = (uint64_t *)calloc((size_t)limit + 1, sizeof(uint64_t));
    if (!F_arr || !G_arr || !spf || !gpf || !cnt) abort();
    for (i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            for (m = i; m <= limit; m += i) {
                if (spf[m] == 0) spf[m] = i;
                gpf[m] = i;
            }
        }
    }
    spf[1] = 1;
    gpf[1] = 1;
    for (i = 1; i <= limit; ++i) {
        cnt[spf[i]]++;
        F_arr[i] = cnt[spf[i]];
    }
    memset(cnt, 0, ((size_t)limit + 1) * sizeof(uint64_t));
    for (i = 1; i <= limit; ++i) {
        cnt[gpf[i]]++;
        G_arr[i] = cnt[gpf[i]];
    }
    free(spf);
    free(gpf);
    free(cnt);
    cap_n = limit;
}

seq_t a(idx_t n)
{
    uint64_t d;
    seq_t s = 0;
    if (n < 1) return 0;
    if (n > cap_n) {
        uint64_t nc = cap_n * 2;
        if (nc < (uint64_t)n) nc = (uint64_t)n;
        if (nc < 1024) nc = 1024;
        build_tables(nc);
    }
    for (d = 1; d <= (uint64_t)n; ++d) {
        if ((uint64_t)n % d == 0) {
            s += (seq_t)F_arr[d] * (seq_t)G_arr[(uint64_t)n / d];
        }
    }
    return s;
}
