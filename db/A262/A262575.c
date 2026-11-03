/*
 * A262575  Concatenation of the numbers from 1 to n but omitting 5.
 * https://oeis.org/A262575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n)=concatenation of the integers from 1 to n with the integer
 *            5 omitted
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-03)
 * Helpers:   mem, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/num_digits_idx.h"
#include "../utils/mem.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* static cache to support out‑of‑order calls */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;        /* largest n that has been computed */
    static idx_t cache_cap  = 0;        /* allocated capacity (>= cache_size+1) */

    if (n == 0)
        return 0;                      /* undefined for n=0 */

    if (n > cache_size) {
        /* ensure enough capacity */
        if (n >= cache_cap) {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 2;
            while (new_cap <= n)
                new_cap *= 2;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) exit(1);         /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute terms up to n */
        seq_t prev = cache_size ? cache[cache_size] : 0;
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            idx_t m = (i < 5 ? i : i + 1);      /* integer to concatenate, 5 omitted */
            int d = num_digits_idx(m);
            seq_t p = pow10(d);
            cache[i] = prev * p + (seq_t)m;
            prev = cache[i];
        }
        cache_size = n;
    }

    return cache[n];
}
