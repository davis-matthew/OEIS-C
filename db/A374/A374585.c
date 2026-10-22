/*
 * A374585  A family of marked Motzkin-like paths.
 * https://oeis.org/A374585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=a(1)=a(2)=a(3)=1 a(n) = 2*a(n-1) - a(n-2) + 2*∑_{k=0}^{n-4}
 *            a(k)*a(n-4-k) for n ≥ 4
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 29, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *seq_cache = NULL;
static size_t seq_cap  = 0;
static idx_t  seq_max  = 3;

static void ensure_capacity(idx_t n)
{
    if (seq_cap < (size_t)(n + 1)) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 4;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(*seq_cache));
        seq_cap   = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n <= 3)
        return (seq_t)1;

    if (seq_cache == NULL) {
        ensure_capacity(3);
        for (idx_t i = 0; i <= 3; ++i)
            seq_cache[i] = (seq_t)1;
        seq_max = 3;
    }

    if (n > seq_max) {
        ensure_capacity(n);
        for (idx_t i = seq_max + 1; i <= n; ++i) {
            seq_t sum = 0;
            idx_t limit = i - 4;
            for (idx_t k = 0; k <= limit; ++k) {
                seq_t prod = seq_cache[k] * seq_cache[limit - k];
                sum += prod;
            }
            seq_t temp = seq_cache[i - 1] * 2;
            temp -= seq_cache[i - 2];
            temp += sum * 2;
            seq_cache[i] = temp;
        }
        seq_max = n;
    }

    return seq_cache[n];
}
