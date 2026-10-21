/*
 * A283094  Number of n X 2 0..1 arrays with no 1 equal to more than one of
 *          its horizontal and vertical neighbors, with the exception of
 *          exactly two elements.
 * https://oeis.org/A283094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 3*a(n-2) - 28*a(n-3) + 6*a(n-4) + 66*a(n-5) +
 *            33*a(n-6) - 36*a(n-7) - 30*a(n-8) + 6*a(n-9) + 9*a(n-10) -
 *            a(n-12) Initial terms (OEIS A283094, offset 1): a(1)=0, a(2)=0,
 *            a(3)=6, a(4)=28, a(5)=142, a(6)=606, a(7)=2458, a(8)=9520,
 *            a(9)=35678, a(10)=130398, a(11)=467036, a(12)=1645468
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>       /* for size_t */

static const idx_t init_vals[13] = {
    0,          /* index 0 unused */
    0,          /* a(1) */
    0,          /* a(2) */
    6,          /* a(3) */
    28,         /* a(4) */
    142,        /* a(5) */
    606,        /* a(6) */
    2458,       /* a(7) */
    9520,       /* a(8) */
    35678,      /* a(9) */
    130398,     /* a(10) */
    467036,     /* a(11) */
    1645468     /* a(12) */
};

static seq_t cache[10001];          /* enough for n up to 10000 */
static idx_t last_computed = 0;     /* track highest computed index */

static void ensure_initialised(void)
{
    if (last_computed >= 12) return;          /* already initialised up to 12 */
    for (idx_t i = last_computed + 1; i <= 12; ++i)
        cache[i] = (seq_t)init_vals[i];
    last_computed = 12;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* outside the offset */

    if (n > 10000) {              /* guard against out‑of‑range first for safety */
        /* For larger n we would need dynamic resizing, but the problem
           specifies n up to approximately 10000. */
        /* In such a case, compute on the fly without caching. */
        seq_t res = 0;
        for (idx_t i = 1; i <= n; ++i) {
            if (i <= 12) res = (seq_t)init_vals[i];
            else {
                seq_t t  = cache[i-1] * (seq_t)6
                         - cache[i-2] * (seq_t)3
                         - cache[i-3] * (seq_t)28
                         + cache[i-4] * (seq_t)6
                         + cache[i-5] * (seq_t)66
                         + cache[i-6] * (seq_t)33
                         - cache[i-7] * (seq_t)36
                         - cache[i-8] * (seq_t)30
                         + cache[i-9] * (seq_t)6
                         + cache[i-10] * (seq_t)9
                         - cache[i-12];
                if (i <= 10000) cache[i] = t;   /* store back if in bounds */
                res = t;
            }
        }
        return res;
    }

    ensure_initialised();

    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            if (i <= 12)                         /* already set in init */
                cache[i] = (seq_t)init_vals[i];
            else {                               /* recurrence */
                cache[i] =
                    cache[i-1] * (seq_t)6
                  - cache[i-2] * (seq_t)3
                  - cache[i-3] * (seq_t)28
                  + cache[i-4] * (seq_t)6
                  + cache[i-5] * (seq_t)66
                  + cache[i-6] * (seq_t)33
                  - cache[i-7] * (seq_t)36
                  - cache[i-8] * (seq_t)30
                  + cache[i-9] * (seq_t)6
                  + cache[i-10] * (seq_t)9
                  - cache[i-12];
            }
        }
        last_computed = n;
    }

    return cache[n];
}
