/*
 * A373172  a(1) = 1; for n >= 2, a(n) = noz(a(n-1) + 4*n - 3), where noz(n) =
 *          A004719(n).
 * https://oeis.org/A373172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1304, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9944 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1;
   for n>=2: a(n) = noz(a(n-1)+4*n-3), where
   noz(x) removes all zero digits from the decimal representation of x. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Helper: remove all zero digits from an integer. */
static seq_t remove_zero_digits(seq_t x)
{
    if (x == 0)
        return 0;  /* By convention, zero becomes zero. */

    /* Collect digits in reverse order. The maximum length needed for
       the sequence values encountered up to n=10000 is well below 256,
       so a fixed-size buffer suffices. */
    unsigned char digits[256];
    int len = 0;
    seq_t t = x;
    while (t != 0) {
        unsigned d = (unsigned)(t % (seq_t)10);
        digits[len++] = (unsigned char)d;
        t /= (seq_t)10;
    }

    /* Reconstruct the number, skipping zero digits. */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i) {
        unsigned d = digits[i];
        if (d != 0)
            result = result * (seq_t)10 + (seq_t)d;
    }
    return result;
}

/* Forward declaration of seq_t and idx_t types (supplied by the build). */

/* Sequence function.  It memoizes previously computed terms so that calls
   with arbitrary n are fast. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* Undefined for n<1; return 0 as a safe value. */

    /* Cache to store computed values.  Index 0 is unused. */
    static seq_t *cache = NULL;      /* The cache array. */
    static idx_t computed = 0;       /* Highest index computed so far. */

    /* Expand cache if necessary. */
    if (n > computed) {
        size_t new_cap = (size_t)(n + 1);          /* Need indices 0..n. */
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) {  /* Allocation failed; return 0. */
            mem_free(cache);
            cache = NULL;
            computed = 0;
            return 0;
        }
        cache = tmp;
        /* Compute terms from computed+1 up to n. */
        for (idx_t i = computed + 1; i <= n; ++i) {
            if (i == 1) {
                cache[1] = (seq_t)1;
            } else {
                seq_t prev = cache[i - 1];
                seq_t add  = ((seq_t)4) * (seq_t)i - ((seq_t)3);
                seq_t sum  = prev + add;
                cache[i]   = remove_zero_digits(sum);
            }
        }
        computed = n;
    }
    return cache[n];
}
