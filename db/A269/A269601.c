/*
 * A269601  Number of length-n 0..2 arrays with no repeated value differing
 *          from the previous repeated value by one or less.
 * https://oeis.org/A269601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 5*a(n-2) - 8*a(n-3) + 12*a(n-4), with a(1)=3,
 *            a(2)=9, a(3)=24, a(4)=62. The function computes the sequence
 *            lazily and memoises all previously computed terms
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 52, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_CACHE 20000

static seq_t cache[MAX_CACHE];
static idx_t computed = 0;

/* compute (and memoise) terms up to *n* */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not used for OEIS offset */

    if (n > MAX_CACHE) {
        /* For n larger than the static cache, fall back to
           iterative computation without memoisation.  This
           branch is rarely needed and keeps the code independent
           of any allocation utilities. */
        seq_t prev4 = 3, prev3 = 9, prev2 = 24, prev1 = 62, cur;
        if (n == 1) return prev4;
        if (n == 2) return prev3;
        if (n == 3) return prev2;
        if (n == 4) return prev1;
        for (idx_t i = 5; i <= n; ++i) {
            cur = prev1 * (seq_t)5 + prev4 * (seq_t)12
                  - prev2 * (seq_t)5 - prev3 * (seq_t)8;
            prev4 = prev3;
            prev3 = prev2;
            prev2 = prev1;
            prev1 = cur;
        }
        return cur;
    }

    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            if (i <= 4) {
                switch (i) {
                    case 1: cache[i - 1] = (seq_t)3;  break;
                    case 2: cache[i - 1] = (seq_t)9;  break;
                    case 3: cache[i - 1] = (seq_t)24; break;
                    case 4: cache[i - 1] = (seq_t)62; break;
                }
            } else {
                seq_t total = cache[i - 2] * (seq_t)5
                            + cache[i - 5] * (seq_t)12;
                seq_t sub   = cache[i - 3] * (seq_t)5
                            + cache[i - 4] * (seq_t)8;
                cache[i - 1] = total - sub;
            }
        }
        if (n > computed) computed = n;
    }
    return cache[n - 1];
}
