/*
 * A244473  3rd-largest term in n-th row of Stern's diatomic triangle A002487.
 * https://oeis.org/A244473
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2) for n>9, with a(0)=0, a(1)=0, a(2)=1,
 *            a(3)=3, a(4)=5, a(5)=11, a(6)=18, a(7)=30, a(8)=49, a(9)=80
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 45, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Pre‑computed initial values for indices 0 … 9 */
static const seq_t base_vals[10] = {0,0,1,3,5,11,18,30,49,80};

/* Cache of computed terms; 10001 is enough for all n up to about 10000 */
static seq_t cache[10001];
static bool initialized = false;
static idx_t max_computed = 9;

/* Return the n‑th term of A244473 (n ≥ 2) */
seq_t a(idx_t n)
{
    if (n < 2)                /* offset is 2 – undefined below */
        return 0;

    if (!initialized) {       /* initialise cache with the first 10 terms */
        for (idx_t i = 0; i <= 9; ++i)
            cache[i] = base_vals[i];
        initialized = true;
    }

    /* compute any missing values up to n */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i)
            cache[i] = cache[i-1] + cache[i-2];
        max_computed = n;
    }

    return cache[n];
}
