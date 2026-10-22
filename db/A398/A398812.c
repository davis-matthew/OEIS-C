/*
 * A398812  a(n) = n^2 - A138808(n): number of lattice points in the square
 *          [1,n] X [1,n] not covered by any box [1,p] X [1,q] with p*q = n.
 * https://oeis.org/A398812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 - A138808(n), where A138808(n) = Σ_{i=1..n} ⌊n /
 *            p_min(i)⌋ and p_min(i) is the smallest divisor of n that is ≥ i
 * Range:     u8 n <= 16, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Integer square root for 64‑bit unsigned integers, safe from overflow. */
static idx_t isqrt_u64(idx_t n) {
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Comparator for qsort when sorting divisor arrays. */
static int cmp_idx_t(const void *A, const void *B) {
    idx_t a = *(const idx_t *)A;
    idx_t b = *(const idx_t *)B;
    return (a > b) - (a < b);
}

/* Compute the nth term of OEIS sequence A398812. */
seq_t a(idx_t n) {
    if (n == 1) return 0;

    /* Gather all divisors of n. */
    idx_t sqrt_n = isqrt_u64(n);
    size_t capacity = sqrt_n * 2 + 2;          /* at most 2*sqrt(n)+2 divisors */
    idx_t *divs = (idx_t *)malloc(capacity * sizeof(idx_t));
    if (!divs) return 0;                       /* allocation failure fallback */

    size_t len = 0;
    for (idx_t i = 1; i <= sqrt_n; ++i) {
        if (n % i == 0) {
            divs[len++] = i;
            if (i != n / i)
                divs[len++] = n / i;
        }
    }

    qsort(divs, len, sizeof(idx_t), cmp_idx_t);

    /* Count covered lattice points. */
    seq_t covered = 0;
    for (size_t j = 0; j + 1 < len; ++j) {
        idx_t d     = divs[j];
        idx_t next  = divs[j + 1];

        /* Contribution of i == d. */
        covered += (seq_t)(n / d);

        /* Contributions for i between d+1 and next-1 (if any). */
        if (next > d + 1) {
            size_t count = (size_t)(next - d - 1);
            covered += (seq_t)count * (seq_t)(n / next);
        }
    }
    /* Add the contribution of the last divisor (i == n). */
    covered += (seq_t)(n / divs[len - 1]);      /* always 1 */

    free(divs);

    seq_t sq = (seq_t)n * (seq_t)n;
    return sq - covered;
}
