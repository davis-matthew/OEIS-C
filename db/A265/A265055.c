/*
 * A265055  Expansion of b(2)*b(4)*b(6)/(x^8-x^4-x+1), where b(k) =
 *          (1-x^k)/(1-x).
 * https://oeis.org/A265055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficients of rational generating function (1 - x - x^4
 *            + x^8) * G(x) = 1 + 3x + 5x^2 + 7x^3 + 8x^4 + 8x^5 + 7x^6 + 5x^7
 *            + 3x^8 + x^9 Recurrence: a(n) = c(n) + a(n-1) + a(n-4) - a(n-8)
 *            where c(0..9) = {1,3,5,7,8,8,7,5,3,1} and c(k)=0 for k>9
 * Range:     u8 n <= 11, u16 n <= 32, u32 n <= 75, u64 n <= 160,
 *            u128 n <= 331, big any n
 * Validated: standard, pass, 61 terms through n = 995 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

static seq_t *cache = NULL;      /* array of computed terms */
static size_t capacity = 0;      /* current allocated size */
static size_t computed = 0;      /* number of terms stored (<= capacity) */

static inline void compute_up_to(idx_t n)
{
    const seq_t COEFF[10] = {
        (seq_t)1, (seq_t)3, (seq_t)5, (seq_t)7,
        (seq_t)8, (seq_t)8, (seq_t)7, (seq_t)5,
        (seq_t)3, (seq_t)1
    };

    size_t target = (size_t)n;           /* index to compute up to, inclusive */
    if (target + 1 <= computed)          /* already computed */
        return;

    /* grow array if necessary */
    if (target + 1 > capacity)
        grow_seq_array(&cache, &capacity, target + 1);

    /* compute missing terms */
    for (size_t i = computed; i <= target; ++i) {
        seq_t val = 0;
        if (i < 10)
            val += COEFF[i];
        if (i >= 1)
            val += cache[i - 1];
        if (i >= 4)
            val += cache[i - 4];
        if (i >= 8)
            val -= cache[i - 8];
        cache[i] = val;
    }

    computed = target + 1;
}

seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[(size_t)n];
}
