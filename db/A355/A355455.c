/*
 * A355455  Numbers whose fourth smallest divisor is a square.
 * https://oeis.org/A355455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Numbers whose fourth smallest divisor is a square (search).
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Integer square‑root for uint64_t (returns floor(sqrt(n))). */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = n;
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if (mid > 0 && mid <= n / mid) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    return low;
}

/* Comparator for qsort on uint64_t. */
static int cmp_uint64(const void *a, const void *b)
{
    const uint64_t *x = (const uint64_t *)a;
    const uint64_t *y = (const uint64_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return  1;
    return 0;
}

/* Return true if the fourth smallest divisor of n is a perfect square. */
static bool fourth_divisor_is_square(uint64_t n)
{
    if (n < 1) return false;

    uint64_t limit = isqrt_u64(n);
    uint64_t divs[128];
    size_t sz = 0;

    for (uint64_t i = 1; i <= limit; ++i) {
        if (n % i == 0) {
            divs[sz++] = i;
            uint64_t j = n / i;
            if (j != i) {
                divs[sz++] = j;
            }
        }
        /* early exit is unnecessary because we need all divisors to sort */
    }

    if (sz < 4) return false;

    qsort(divs, sz, sizeof(uint64_t), cmp_uint64);

    uint64_t fourth = divs[3];
    uint64_t r = isqrt_u64(fourth);
    return r * r == fourth;
}

/* Memoised storage for the sequence. */
static seq_t *terms = NULL;      /* array of terms */
static size_t terms_cap = 0;      /* capacity of the array */
static size_t terms_len = 0;      /* number of terms stored */
static uint64_t last_checked = 0; /* last integer that was examined */

/* Grow the terms array to at least 'needed' elements. */
static void grow_terms(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t new_cap = (terms_cap * 2) + 16;
    seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_terms) exit(1);  /* allocation failure → abort */
    terms = new_terms;
    terms_cap = new_cap;
}

/* Ensure that at least 'n' terms are computed. */
static void ensure_terms(size_t n)
{
    while (terms_len < n) {
        ++last_checked;
        if (fourth_divisor_is_square(last_checked)) {
            grow_terms(terms_len + 1);
            terms[terms_len++] = (seq_t)last_checked;
        }
    }
}

/* Required external function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* behaviour below offset is irrelevant */
    ensure_terms((size_t)n);
    return terms[(size_t)n - 1];
}
