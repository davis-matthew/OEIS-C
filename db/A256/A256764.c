/*
 * A256764  Number of (n+2)X(1+2) 0..1 arrays with no 3x3 subblock diagonal
 *          sum less than the antidiagonal sum.
 * https://oeis.org/A256764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) -144*a(n-3) +272*a(n-4) +704*a(n-5) -2304*a(n-6)
 *            +5120*a(n-8) -4096*a(n-9) Initial values: a(1)=352, a(2)=1936,
 *            a(3)=9152, a(4)=43264, a(5)=189696, a(6)=831744, a(7)=3502080,
 *            a(8)=14745600, a(9)=60702720
 * Range:     u8 none, u16 n <= 4, u32 n <= 12, u64 n <= 28, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* Compute A256764: number of (n+2)×(1+2) 0..1 arrays with no 3×3
   subblock diagonal sum less than the antidiagonal sum. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* 1‑based cache of computed terms */
    static size_t cache_cap = 0;    /* allocated capacity (index 0..cache_cap‑1) */

    size_t nn = (size_t)n;          /* Size_t used for array indices */

    /* Ensure cache can hold terms up to n */
    if (n == 0)
        return 0; /* undefined for n < 1, but harmless */
    if (nn + 1 > cache_cap) {
        size_t new_cap = nn + 1;
        seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_arr)                /* OOM – terminate */
            abort();
        cache = new_arr;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (size_t i = 1; i <= nn; ++i) {
        if (i <= 9) {   /* initial values for 1 ≤ n ≤ 9 */
            switch (i) {
            case 1: cache[1] = (seq_t)352;      break;
            case 2: cache[2] = (seq_t)1936;     break;
            case 3: cache[3] = (seq_t)9152;     break;
            case 4: cache[4] = (seq_t)43264;    break;
            case 5: cache[5] = (seq_t)189696;   break;
            case 6: cache[6] = (seq_t)831744;   break;
            case 7: cache[7] = (seq_t)3502080;  break;
            case 8: cache[8] = (seq_t)14745600; break;
            case 9: cache[9] = (seq_t)60702720; break;
            }
        } else if (i > 9) {
            seq_t pos = 0;
            pos += 8   * cache[i - 1];
            pos += 272 * cache[i - 4];
            pos += 704 * cache[i - 5];
            pos += 5120 * cache[i - 8];
            seq_t neg = 0;
            neg += 144   * cache[i - 3];
            neg += 2304  * cache[i - 6];
            neg += 4096  * cache[i - 9];
            cache[i] = pos - neg;   /* pos > neg for all n ≥ 10   */
        }
    }

    return cache[nn];
}
