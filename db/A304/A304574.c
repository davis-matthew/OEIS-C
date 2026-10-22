/*
 * A304574  Number of perfect powers (A001597) less than n and relatively
 *          prime to n.
 * https://oeis.org/A304574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ m < n | m is a perfect power and gcd(m,n)=1 }
 * Range:     u8 n <= 324, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Integer square root: floor(sqrt(x)) for unsigned 64‑bit integer. */
static idx_t isqrt_u64(idx_t x)
{
    if (x == 0) return 0;
    idx_t high = (idx_t)1 << 32;
    idx_t low = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t div = x / mid;
        if (mid > div) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return high;
}

/* Comparator for qsort on idx_t. */
static int cmp_idx_t(const void *a, const void *b)
{
    idx_t v1 = *(const idx_t *)a;
    idx_t v2 = *(const idx_t *)b;
    return (v1 > v2) - (v1 < v2);
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;               /* a(1)=0, n<1 is undefined but return 0 */

    /* Allocate storage for potential perfect powers. */
    idx_t *powers = NULL;
    size_t cap = 0, len = 0;

#define PUSH(v)                                          \
    do {                                                  \
        if (len == cap) {                                 \
            size_t newcap = cap ? cap << 1 : 256;         \
            idx_t *tmp = (idx_t *)realloc(powers, newcap * sizeof(idx_t)); \
            if (!tmp) { free(powers); return 0; }         \
            powers = tmp; cap = newcap;                  \
        }                                                 \
        powers[len++] = (v);                              \
    } while (0)

    /* 1 is a perfect power. */
    PUSH(1);

    /* Enumerate higher perfect powers. */
    idx_t max_base = isqrt_u64(n - 1);
    for (idx_t base = 2; base <= max_base; ++base) {
        idx_t power = base * base;          /* exponent 2 */
        while (power < n) {
            PUSH(power);
            if (power > (n - 1) / base) break;   /* prevent overflow */
            power *= base;
        }
    }

    /* Sort and deduplicate, then count those coprime to n. */
    qsort(powers, len, sizeof(idx_t), cmp_idx_t);
    seq_t count = 0;
    for (size_t i = 0; i < len; ++i) {
        if (i > 0 && powers[i] == powers[i - 1]) continue; /* duplicate */
        if (gcd((seq_t)powers[i], (seq_t)n) == 1) {
            count++;
        }
    }

    free(powers);
    return count;
}
