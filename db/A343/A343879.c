/*
 * A343879  Number of pairs (d1, d2) of divisors of n such that d1<d2, d1|n,
 *          d2|n, d1|d2 and d1 + d2 <= n.
 * https://oeis.org/A343879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1439, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A343879 (OEIS A343879) – number of divisor pairs (d1, d2)
   of n with d1 < d2, d1 | d2, and d1 + d2 <= n – via brute‑force enumeration
   of all divisors of n and testing each pair. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Integer square root for 64‑bit values.  Uses binary search avoiding
   multiplication overflow by comparing with n / mid. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {
            low = 1;
            continue;
        }
        if (mid <= n / mid) {            /* mid*mid <= n */
            if (mid * mid == n)          /* exact square */
                return mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    /* Allocate a dynamic array for the divisors.  Capacity grows by
       doubling whenever needed. */
    size_t cap = 64;
    idx_t *divs = (idx_t *)malloc(cap * sizeof(idx_t));
    if (!divs)
        return (seq_t)0;      /* allocation failure – safe default */

    size_t divcnt = 0;
    idx_t root = isqrt_u64(n);
    for (idx_t d = 1; d <= root; ++d) {
        if (n % d == 0) {
            if (divcnt == cap) {
                cap <<= 1;
                idx_t *tmp = (idx_t *)realloc(divs, cap * sizeof(idx_t));
                if (!tmp) {
                    free(divs);
                    return (seq_t)0;
                }
                divs = tmp;
            }
            divs[divcnt++] = d;
            idx_t other = n / d;
            if (other != d) {
                if (divcnt == cap) {
                    cap <<= 1;
                    idx_t *tmp = (idx_t *)realloc(divs, cap * sizeof(idx_t));
                    if (!tmp) {
                        free(divs);
                        return (seq_t)0;
                    }
                    divs = tmp;
                }
                divs[divcnt++] = other;
            }
        }
    }

    seq_t cnt = 0;
    for (size_t i = 0; i < divcnt; ++i) {
        idx_t d1 = divs[i];
        for (size_t j = i + 1; j < divcnt; ++j) {
            idx_t d2 = divs[j];
            idx_t small = d1 < d2 ? d1 : d2;
            idx_t big   = d1 < d2 ? d2 : d1;
            if (big % small != 0)
                continue;
            if (small <= (idx_t)n - big)
                ++cnt;
        }
    }
    free(divs);
    return cnt;
}
