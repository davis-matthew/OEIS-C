/*
 * A274658  Irregular triangle which lists in row n the divisors of 2*n+1.
 * https://oeis.org/A274658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) is the n‑th element of the concatenation of rows * of
 *            divisors of 2*k+1. Row k lists all divisors of 2*k+1 in *
 *            increasing order. The function returns the term at index n
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Comparison function for qsort: sort uint64_t values ascending. */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a;
    uint64_t y = *(const uint64_t *)b;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

/* Factorise n (n <= 20001 in the intended usage) into primes.
 *  primes[i]   – i‑th prime factor
 *  exps[i]     – exponent of that prime
 *  *cnt        – number of distinct prime factors
 */
static void factor_uint64(uint64_t n, uint64_t *primes, uint64_t *exps, size_t *cnt)
{
    *cnt = 0;
    if ((n & 1ULL) == 0) {                 /* factor 2 */
        primes[*cnt] = 2;
        exps[*cnt] = 0;
        while ((n & 1ULL) == 0) {
            exps[*cnt]++;
            n >>= 1;
        }
        (*cnt)++;
    }
    for (uint64_t f = 3; f * f <= n; f += 2) {
        if (n % f == 0) {
            primes[*cnt] = f;
            exps[*cnt] = 0;
            while (n % f == 0) {
                exps[*cnt]++;
                n /= f;
            }
            (*cnt)++;
        }
    }
    if (n > 1) {                           /* remaining prime factor */
        primes[*cnt] = n;
        exps[*cnt] = 1;
        (*cnt)++;
    }
}

/* Number of divisors of val (val is odd, <= 20001). */
static idx_t divisor_count(uint64_t val)
{
    uint64_t primes[32], exps[32];
    size_t cnt;
    factor_uint64(val, primes, exps, &cnt);
    idx_t d = 1;
    for (size_t i = 0; i < cnt; ++i)
        d *= ((idx_t)exps[i] + 1);
    return d;
}

/* Return the k‑th divisor (0‑based) of val in increasing order. */
static seq_t kth_divisor(uint64_t val, idx_t k)
{
    uint64_t primes[32], exps[32];
    size_t cnt;
    factor_uint64(val, primes, exps, &cnt);

    uint64_t divs[512];                     /* enough for val ≤ 20001 */
    size_t used = 1;
    divs[0] = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        size_t cur = used;
        for (size_t j = 0; j < cur; ++j) {
            uint64_t base = divs[j];
            uint64_t mul  = 1;
            for (uint64_t h = 0; h < e; ++h) {
                mul *= p;
                divs[used++] = base * mul;
            }
        }
    }
    qsort(divs, used, sizeof(uint64_t), cmp_uint64);
    return (seq_t)divs[(size_t)k];
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* static cumulative divisor count: cum[m] = total terms in rows 0..m */
    static idx_t *cum = NULL;
    static size_t cap = 0;

    if (cum == NULL) {                /* first call: initialise */
        cap = 1;
        cum = (idx_t *)malloc(cap * sizeof(idx_t));
        if (!cum) return 0;
        cum[0] = divisor_count(1);    /* row 0: divisors of 1 */
    }

    /* Grow array until it contains an entry beyond n. */
    while (n >= cum[cap - 1]) {
        size_t old_cap = cap;
        cap <<= 1;                     /* double capacity */
        cum = (idx_t *)realloc(cum, cap * sizeof(idx_t));
        for (size_t m = old_cap; m < cap; ++m) {
            uint64_t val = 2 * m + 1;
            idx_t d = divisor_count(val);
            cum[m] = cum[m - 1] + d;
        }
    }

    /* Find row m such that cum[m‑1] ≤ n < cum[m]  (binary search). */
    size_t lo = 0, hi = cap - 1;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (n < cum[mid])
            hi = mid;
        else
            lo = mid + 1;
    }
    size_t m = lo;
    idx_t prev = (m == 0) ? 0 : cum[m - 1];
    idx_t offset = n - prev;                        /* 0‑based offset in row m */

    uint64_t val = 2 * m + 1;
    return kth_divisor(val, offset);
}
