/*
 * A378820  a(n) is the number of distinct nondegenerate triangles whose sides
 *          are divisors of n.
 * https://oeis.org/A378820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 179, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count distinct nondegenerate triangles with sides that divide n.
   For each unordered triple (a,b,c) with a<=b<=c and a+b>c,
   a,b,c are divisors of n.  The function enumerates all divisor
   permutations in sorted order to avoid duplicates. */

#include <stdlib.h>
#include <stdint.h>

/* comparison function for qsort (ascending idx_t) */
static int cmp_idx_t(const void *p1, const void *p2)
{
    const idx_t *a = (const idx_t *)p1;
    const idx_t *b = (const idx_t *)p2;
    return (*a > *b) - (*a < *b);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* safety, though n>=1 per OEIS */

    /* find sqrt(n) by integer loop */
    idx_t sqrt_n = 0;
    for (idx_t i = 1; i * i <= n; ++i)
        sqrt_n = i;

    /* collect divisors */
    size_t cap = 16;
    size_t len = 0;
    idx_t *divs = (idx_t *)malloc(cap * sizeof(idx_t));
    if (!divs) return 0; /* failure */

    for (idx_t i = 1; i <= sqrt_n; ++i) {
        if (n % i == 0) {
            if (len >= cap) {
                cap *= 2;
                idx_t *tmp = (idx_t *)realloc(divs, cap * sizeof(idx_t));
                if (!tmp) { free(divs); return 0; }
                divs = tmp;
            }
            divs[len++] = i;
            idx_t j = n / i;
            if (j != i) {
                if (len >= cap) {
                    cap *= 2;
                    idx_t *tmp = (idx_t *)realloc(divs, cap * sizeof(idx_t));
                    if (!tmp) { free(divs); return 0; }
                    divs = tmp;
                }
                divs[len++] = j;
            }
        }
    }

    /* sort divisors */
    qsort(divs, len, sizeof(idx_t), cmp_idx_t);

    /* count valid triangles */
    seq_t count = 0;
    for (size_t i = 0; i < len; ++i) {
        idx_t a_ = divs[i];
        for (size_t j = i; j < len; ++j) {
            idx_t b_ = divs[j];
            for (size_t k = j; k < len; ++k) {
                idx_t c_ = divs[k];
                if (a_ + b_ > c_) count++;
            }
        }
    }

    free(divs);
    return count;
}
