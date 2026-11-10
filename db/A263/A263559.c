/*
 * A263559  a(n) = A083186(n) mod A007504(n).
 * https://oeis.org/A263559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A083186(n) mod A007504(n), where A083186(n) =
 *            Sum_{k=1..n} prime(prime(k)) and A007504(n) = Sum_{k=1..n}
 *            prime(k); computed with a doubling sieve
 * Range:     u8 n <= 8, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static uint64_t *a263559_pr = NULL;
static uint64_t a263559_cnt = 0;
static uint64_t a263559_lim = 0;

static void a263559_sieve(uint64_t limit) {
    unsigned char *comp = (unsigned char *)malloc((size_t)(limit + 1));
    uint64_t i, j, c = 0;
    memset(comp, 0, (size_t)(limit + 1));
    for (i = 2; i * i <= limit; i++)
        if (!comp[i])
            for (j = i * i; j <= limit; j += i) comp[j] = 1;
    for (i = 2; i <= limit; i++) if (!comp[i]) c++;
    free(a263559_pr);
    a263559_pr = (uint64_t *)malloc((size_t)(c ? c : 1) * sizeof(uint64_t));
    c = 0;
    for (i = 2; i <= limit; i++) if (!comp[i]) a263559_pr[c++] = i;
    free(comp);
    a263559_cnt = c;
    a263559_lim = limit;
}

seq_t a(idx_t n) {
    seq_t s1 = 0, s2 = 0;
    uint64_t k;
    if (n == 0) return 0;
    for (;;) {
        if (a263559_cnt >= (uint64_t)n && a263559_pr[n - 1] <= a263559_cnt) break;
        a263559_sieve(a263559_lim ? a263559_lim * 2 : 4096);
    }
    for (k = 0; k < (uint64_t)n; k++) {
        s2 += (seq_t)a263559_pr[k];
        s1 += (seq_t)a263559_pr[a263559_pr[k] - 1];
    }
    return s1 % s2;
}
