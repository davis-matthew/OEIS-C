/*
 * A328511  Number of non-singleton runs of divisors of 2n.
 * https://oeis.org/A328511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of non‑singleton runs of the divisors of 2n. */
#include <stdlib.h>
#include <stdint.h>

/* Computes floor(sqrt(n)) for unsigned 64‑bit integers. */
static idx_t
isqrt_u64(idx_t n)
{
    idx_t low = 0, high = (n >> 1) + 1, mid;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        if ((__uint128_t)mid * mid <= n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return low - 1;
}

/* Comparator for qsort on idx_t values. */
static int
cmp_idx(const void *a, const void *b)
{
    const idx_t v1 = *(const idx_t *)a;
    const idx_t v2 = *(const idx_t *)b;
    return (v1 < v2) ? -1 : (v1 > v2);
}

/* seq_t a(idx_t n) == number of non‑singleton runs of divisors of 2n. */
seq_t
a(idx_t n)
{
    /* m = 2n: integer whose divisors we analyze. */
    idx_t m = n << 1;                       /* 2*n, safe for n < 2^63 */

    /* Dynamic array for divisors. */
    size_t cap = 16;
    size_t cnt = 0;
    idx_t *divs = (idx_t *)malloc(cap * sizeof(idx_t));
    if (!divs) return 0; /* allocation failure: return 0 */

    /* Enumerate divisors up to floor(sqrt(m)). */
    idx_t limit = isqrt_u64(m);
    for (idx_t d = 1; d <= limit; ++d) {
        if (m % d == 0) {
            if (cnt == cap) {
                cap <<= 1;
                idx_t *tmp = (idx_t *)realloc(divs, cap * sizeof(idx_t));
                if (!tmp) { free(divs); return 0; }
                divs = tmp;
            }
            divs[cnt++] = d;
            idx_t e = m / d;
            if (e != d) {
                if (cnt == cap) {
                    cap <<= 1;
                    idx_t *tmp = (idx_t *)realloc(divs, cap * sizeof(idx_t));
                    if (!tmp) { free(divs); return 0; }
                    divs = tmp;
                }
                divs[cnt++] = e;
            }
        }
    }

    /* Sort the divisor list. */
    qsort(divs, cnt, sizeof(idx_t), cmp_idx);

    /* Count runs of consecutive divisors of length >= 2. */
    seq_t res = 0;
    size_t runlen = 1;
    for (size_t i = 1; i < cnt; ++i) {
        if (divs[i] == divs[i - 1] + 1) {
            ++runlen;
        } else {
            if (runlen >= 2) res = res + 1;
            runlen = 1;
        }
    }
    if (runlen >= 2) res = res + 1;

    free(divs);
    return res;
}
