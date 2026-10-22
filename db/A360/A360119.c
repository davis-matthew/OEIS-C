/*
 * A360119  Number of divisors of n which are not also differences between
 *          consecutive divisors, minus the number of differences between
 *          consecutive divisors of n which are not also divisors of n. Here
 *          the differences are counted with repetition if they occur more
 *          than once.
 * https://oeis.org/A360119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A060764(n) - A360118(n) (number of divisors not equal to
 *            a difference minus number of differences not equal to a divisor)
 * Range:     u8 n <= 9240, u16 n <= 9240, u32 n <= 9240, u64 n <= 9240,
 *            u128 n <= 9240, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* comparison function for qsort on idx_t */
static int cmp_idx_t(const void *p1, const void *p2)
{
    const idx_t *a = (const idx_t *)p1;
    const idx_t *b = (const idx_t *)p2;
    return (*a > *b) - (*a < *b);
}

/* binary search: true if val appears in sorted array arr[0..n-1] */
static bool binary_search(const idx_t *arr, size_t n, idx_t val)
{
    size_t lo = 0, hi = n;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        idx_t mv = arr[mid];
        if (mv == val) return true;
        if (mv < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return false;
}

/* A360119 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* offset starts at 1 */

    /* ---- collect divisors of n ---- */
    size_t cap = 16, len = 0;
    idx_t *divs = (idx_t *)malloc(cap * sizeof(idx_t));
    if (!divs) return (seq_t)0;            /* allocation failure */

    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            if (len == cap) {
                cap <<= 1;
                idx_t *tmp = (idx_t *)realloc(divs, cap * sizeof(idx_t));
                if (!tmp) { free(divs); return (seq_t)0; }
                divs = tmp;
            }
            divs[len++] = d;
            if (d != n / d) {
                if (len == cap) {
                    cap <<= 1;
                    idx_t *tmp = (idx_t *)realloc(divs, cap * sizeof(idx_t));
                    if (!tmp) { free(divs); return (seq_t)0; }
                    divs = tmp;
                }
                divs[len++] = n / d;
            }
        }
    }

    /* sort divisors */
    qsort(divs, len, sizeof(idx_t), cmp_idx_t);

    if (len == 1) {                 /* n = 1 */
        free(divs);
        return (seq_t)1;
    }

    /* ---- compute consecutive differences ---- */
    size_t dlen = len - 1;
    idx_t *diffs = (idx_t *)malloc(dlen * sizeof(idx_t));
    if (!diffs) { free(divs); return (seq_t)0; }

    for (size_t i = 0; i < dlen; ++i)
        diffs[i] = divs[i + 1] - divs[i];

    /* sort differences */
    qsort(diffs, dlen, sizeof(idx_t), cmp_idx_t);

    /* ---- counting ---- */
    uint64_t div_not_in_diff = 0;
    uint64_t diff_not_in_div = 0;

    for (size_t i = 0; i < len; ++i)
        if (!binary_search(diffs, dlen, divs[i]))
            ++div_not_in_diff;

    for (size_t i = 0; i < dlen; ++i)
        if (!binary_search(divs, len, diffs[i]))
            ++diff_not_in_div;

    free(divs);
    free(diffs);

    return (seq_t)(div_not_in_diff - diff_not_in_div);
}
