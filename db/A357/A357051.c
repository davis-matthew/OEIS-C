/*
 * A357051  a(n) = Sum_{d|n} 3^(n-d).
 * https://oeis.org/A357051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{d|n} 3^(n-d)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS A357051. */

#include <stdlib.h>
#include <stdint.h>

/* Allocate and extend the 3^k cache as needed. */
static seq_t *pow3_cache = NULL;   /* power of 3 array: pow3_cache[k] = 3^k */
static idx_t pow3_cap = 0;        /* current capacity (last allocated index + 1) */

/* Ensure pow3_cache contains 3^k for all 0 <= k <= n. */
static void ensure_pow3(idx_t n)
{
    if (pow3_cap == 0) {
        /* initial allocation for exponent 0 */
        pow3_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!pow3_cache) exit(1);
        pow3_cache[0] = (seq_t)1;
        pow3_cap = 1;
    }
    if (n >= pow3_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(pow3_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        pow3_cache = tmp;
        /* compute missing powers */
        for (idx_t i = pow3_cap; i <= n; ++i)
            pow3_cache[i] = pow3_cache[i - 1] * (seq_t)3;
        pow3_cap = (idx_t)new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset starts at 1, but still handle 0 */
    ensure_pow3(n);            /* make sure 3^k values are ready */

    seq_t sum = 0;
    idx_t i;
    for (i = 1; i <= n / i; ++i) {   /* avoid overflow in i*i */
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            sum += pow3_cache[n - d1];
            if (d1 != d2)
                sum += pow3_cache[n - d2];
        }
    }
    return sum;
}
