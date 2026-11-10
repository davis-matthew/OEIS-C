/*
 * A325441  Complement of A325440.
 * https://oeis.org/A325441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th positive integer not in A325440, where A325440 is
 *            built greedily (least number not 2*a(m)-1 or 3*a(m)-1 for
 *            earlier m); computed by sieve
 * Range:     u8 n <= 101, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9237 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static unsigned long long *A325441_cache = NULL;
static size_t A325441_count = 0;

static void A325441_build(size_t need)
{
    size_t L = 64;
    for (;;) {
        bool *forb = (bool *)calloc(L + 1, sizeof(bool));
        unsigned long long *res = (unsigned long long *)malloc((L + 1) * sizeof(unsigned long long));
        size_t cnt = 0;
        if (!forb || !res) abort();
        for (size_t x = 1; x <= L; ++x) {
            if (forb[x]) {
                res[cnt++] = (unsigned long long)x;
            } else {
                size_t f1 = 2 * x - 1, f2 = 3 * x - 1;
                if (f1 <= L) forb[f1] = true;
                if (f2 <= L) forb[f2] = true;
            }
        }
        free(forb);
        if (cnt >= need) {
            free(A325441_cache);
            A325441_cache = res;
            A325441_count = cnt;
            return;
        }
        free(res);
        L <<= 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if ((size_t)n > A325441_count) A325441_build((size_t)n);
    return (seq_t)A325441_cache[(size_t)n - 1];
}
