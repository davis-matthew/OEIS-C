/*
 * A316519  a(n) is the number of distinct nonzero values frac(n/k) can take
 *          for k in 1..n, where frac denotes the fractional part.
 * https://oeis.org/A316519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 261, u16 n <= 9920, u32 n <= 9920, u64 n <= 9920,
 *            u128 n <= 9920, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts distinct nonzero fractional parts of n/k for k=1..n
   by enumerating all k, reducing each fraction (n mod k)/k to
   lowest terms, and counting unique reduced fractions. */
#include <stdlib.h>
#include <stdint.h>

/* Euclidean GCD for idx_t. */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Structure to hold a reduced fraction num/den. */
struct frac { idx_t num; idx_t den; };

/* Compare two fractions by cross multiplication. */
static int cmp_frac(const void *p1, const void *p2)
{
    const struct frac *f1 = (const struct frac*)p1;
    const struct frac *f2 = (const struct frac*)p2;
    uint64_t left  = (uint64_t)f1->num * f2->den;
    uint64_t right = (uint64_t)f2->num * f1->den;
    if (left < right) return -1;
    if (left > right) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    /* Allocate space for at most n reduced fractions. */
    struct frac *arr = (struct frac*)malloc((size_t)n * sizeof(struct frac));
    if (!arr) return (seq_t)0;  /* out‑of‑memory safe fallback */

    size_t cnt = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t r = n % k;
        if (r == 0) continue;                /* skip zero fractions */
        idx_t g = gcd_idx(r, k);
        arr[cnt].num = r / g;
        arr[cnt].den = k / g;
        ++cnt;
    }
    if (cnt == 0) { free(arr); return (seq_t)0; }

    /* Sort the fractions to enable uniqueness counting. */
    qsort(arr, cnt, sizeof(struct frac), cmp_frac);

    /* Count unique reduced fractions. */
    size_t unique = 1;
    for (size_t i = 1; i < cnt; ++i) {
        if (cmp_frac(&arr[i - 1], &arr[i]) != 0) ++unique;
    }

    free(arr);
    return (seq_t)unique;
}
