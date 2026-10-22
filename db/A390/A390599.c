/*
 * A390599  Smallest number obtained by concatenating a permutation of the
 *          divisors of n.
 * https://oeis.org/A390599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 29, u64 n <= 119, u128 n <= 359,
 *            big any n
 * Helpers:   digits_of, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = smallest number obtained by concatenating a permutation
 * of the divisors of n.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/digits_of.h"

/* Comparator for sorting divisors to minimize concatenation */
static int cmp_divisors(const void *va, const void *vb)
{
    uint64_t a = *(const uint64_t *)va;
    uint64_t b = *(const uint64_t *)vb;
    int la = digits_of(a);
    int lb = digits_of(b);
    seq_t val1 = (seq_t)a * pow10((idx_t)lb) + (seq_t)b;
    seq_t val2 = (seq_t)b * pow10((idx_t)la) + (seq_t)a;
    if (val1 < val2) return -1;
    if (val1 > val2) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety for out‑of‑bound indices */

    /* First pass: count divisors */
    idx_t count = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            ++count;
            if (i != n / i) ++count;
        }
    }

    /* Allocate array for divisors */
    uint64_t *divs = (uint64_t *)malloc(count * sizeof(uint64_t));
    if (!divs) return 0;   /* allocation failure */

    /* Second pass: store divisors */
    idx_t pos = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            divs[pos++] = i;
            if (i != n / i) divs[pos++] = n / i;
        }
    }

    /* Sort divisors with custom rule */
    qsort(divs, count, sizeof(uint64_t), cmp_divisors);

    /* Concatenate in sorted order to obtain minimal value */
    seq_t result = 0;
    for (idx_t k = 0; k < count; ++k) {
        uint64_t d = divs[k];
        int len = digits_of(d);
        result = result * pow10((idx_t)len) + (seq_t)d;
    }

    free(divs);
    return result;
}
