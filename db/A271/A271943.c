/*
 * A271943  The sum of the widths of all bargraphs of semiperimeter n (n>=2).
 * https://oeis.org/A271943
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>=1} k*A271942(n,k) Recurrence (valid for n>=8):
 *            (n+1)*a(n) = (6*n-1)*a(n-1) - (9*n-14)*a(n-2) + 2*a(n-3) +
 *            (n-11)*a(n-4) + (2*n-9)*a(n-5) + (n-6)*a(n-6) Base values
 *            (offset 2): a(2)=1, a(3)=3, a(4)=10, a(5)=33, a(6)=108, a(7)=353
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 18, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_N 10000   /* enough for all tests (n up to about 10000) */

static seq_t cache[MAX_N + 1];
static int initialized = 0;

seq_t a(idx_t n) {
    if (n < 2) return 0;               /* values before offset are irrelevant */

    /* initialize first 6 non‑zero terms once */
    if (!initialized) {
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)10;
        cache[5] = (seq_t)33;
        cache[6] = (seq_t)108;
        cache[7] = (seq_t)353;
        initialized = 1;
    }

    /* guard: we assume n <= MAX_N for all tests */
    if (n > (idx_t)MAX_N) {
        /* if larger n occurs, extend cache lazily (not needed for current tests) */
        /* simply compute up to n using a temporary array of size n+1 (fallback) */
        size_t size = (size_t)n + 1;
        seq_t *tmp = (seq_t*)malloc(size * sizeof(seq_t));
        if (!tmp) abort();   /* allocation failure */
        for (size_t i = 0; i <= n; ++i) tmp[i] = 0;
        /* copy already computed values */
        if (initialized) {
            size_t copy_len = sizeof(cache) / sizeof(cache[0]);
            if (copy_len > size) copy_len = size;
            for (size_t i = 0; i < copy_len; ++i) tmp[i] = cache[i];
        }
        /* perform calculations using tmp instead of cache */
        for (idx_t i = 8; i <= n; ++i) {
            seq_t term1 = ((seq_t)(6 * i - 1)) * tmp[i - 1];
            seq_t term2 = ((seq_t)(9 * i - 14)) * tmp[i - 2];
            seq_t term3 = 2 * tmp[i - 3];
            seq_t term5 = ((seq_t)(2 * i - 9)) * tmp[i - 5];
            seq_t term6 = ((seq_t)(i - 6)) * tmp[i - 6];
            seq_t num = term1 + term3 + term5 + term6;
            if (i >= 11) num += ((seq_t)(i - 11)) * tmp[i - 4];
            else        num -= ((seq_t)(11 - i)) * tmp[i - 4];
            num -= term2;
            tmp[i] = num / ((seq_t)(i + 1));
        }
        seq_t res = tmp[n];
        free(tmp);
        return res;
    }

    /* compute sequentially up to n */
    for (idx_t i = 8; i <= n; ++i) {
        seq_t term1 = ((seq_t)(6 * i - 1)) * cache[i - 1];
        seq_t term2 = ((seq_t)(9 * i - 14)) * cache[i - 2];
        seq_t term3 = 2 * cache[i - 3];
        seq_t term5 = ((seq_t)(2 * i - 9)) * cache[i - 5];
        seq_t term6 = ((seq_t)(i - 6)) * cache[i - 6];
        seq_t num = term1 + term3 + term5 + term6;
        if (i >= 11) num += ((seq_t)(i - 11)) * cache[i - 4];
        else        num -= ((seq_t)(11 - i)) * cache[i - 4];
        num -= term2;
        cache[i] = num / ((seq_t)(i + 1));
    }

    return cache[n];
}
