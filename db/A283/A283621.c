/*
 * A283621  Smallest term A001043(k) with k>n such that A001043(k) is multiple
 *          of A001043(n).
 * https://oeis.org/A283621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001043(k) for the least k > n with A001043(n) |
 *            A001043(k), where A001043(k) = prime(k)+prime(k+1); found by a
 *            search over a prime sieve
 * Range:     u8 n <= 8, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static uint64_t *a283621_pr = NULL;
static uint64_t a283621_cnt = 0;
static uint64_t a283621_lim = 0;

static void a283621_sieve(uint64_t limit)
{
    unsigned char *comp = (unsigned char *)malloc((size_t)limit + 1);
    uint64_t i, j, c = 0;
    memset(comp, 0, (size_t)limit + 1);
    for (i = 2; i * i <= limit; i++)
        if (!comp[i])
            for (j = i * i; j <= limit; j += i)
                comp[j] = 1;
    for (i = 2; i <= limit; i++)
        if (!comp[i]) c++;
    free(a283621_pr);
    a283621_pr = (uint64_t *)malloc((size_t)(c ? c : 1) * sizeof(uint64_t));
    c = 0;
    for (i = 2; i <= limit; i++)
        if (!comp[i]) a283621_pr[c++] = i;
    free(comp);
    a283621_cnt = c;
    a283621_lim = limit;
}

static void a283621_ensure(uint64_t k)
{
    uint64_t limit = a283621_lim ? a283621_lim : 4096;
    while (a283621_cnt < k) {
        if (a283621_lim) limit *= 2;
        a283621_sieve(limit);
    }
}

seq_t a(idx_t n)
{
    uint64_t base, k, s;
    a283621_ensure((uint64_t)n + 1);
    base = a283621_pr[n - 1] + a283621_pr[n];
    for (k = (uint64_t)n + 1;; k++) {
        a283621_ensure(k + 1);
        s = a283621_pr[k - 1] + a283621_pr[k];
        if (s % base == 0)
            return (seq_t)s;
    }
}
