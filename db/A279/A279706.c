/*
 * A279706  Number of nX5 0..1 arrays with no element equal to a strict
 *          majority of its horizontal and antidiagonal neighbors and with new
 *          values introduced in order 0 sequentially upwards.
 * https://oeis.org/A279706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A279706
    Empirical recurrence (valid for n > 13):
        a(n) = 17*a(n-1) -107*a(n-2) +388*a(n-3) -991*a(n-4)
               +1845*a(n-5) -2506*a(n-6) +2365*a(n-7) -1546*a(n-8)
               +695*a(n-9) -219*a(n-10) +44*a(n-11) -4*a(n-12)
    Initial values (for 1 ≤ n ≤ 13):
        a(1)=3, a(2)=6, a(3)=47, a(4)=385, a(5)=3245, a(6)=27346,
        a(7)=230128, a(8)=1936687, a(9)=16300179, a(10)=137192011,
        a(11)=1154685911, a(12)=9718495665, a(13)=81796457512
*/

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */
#include <stdlib.h>   /* malloc, realloc, abort */

static const int coeff[12] = {
    17,  -107, 388, -991, 1845, -2506, 2365, -1546, 695, -219, 44, -4
};

static seq_t init_vals[14];
static int init_done = 0;

static void init_table(void)
{
    if (init_done) return;
    init_vals[0] = 0;
    init_vals[1] = (seq_t)3;
    init_vals[2] = (seq_t)6;
    init_vals[3] = (seq_t)47;
    init_vals[4] = (seq_t)385;
    init_vals[5] = (seq_t)3245;
    init_vals[6] = (seq_t)27346;
    init_vals[7] = (seq_t)230128;
    init_vals[8] = (seq_t)1936687;
    init_vals[9] = (seq_t)16300179;
    init_vals[10] = (seq_t)137192011;
    init_vals[11] = (seq_t)1154685911;
    init_vals[12] = (seq_t)9718495665ULL;
    init_vals[13] = (seq_t)81796457512ULL;
    init_done = 1;
}

static seq_t *cache      = NULL;
static size_t cache_sz   = 0;
static idx_t  cache_max  = 0;

seq_t a(idx_t n)
{
    init_table();

    if (n <= 13) return init_vals[n];

    /* create or enlarge the cache if needed */
    if (!cache || cache_sz <= (size_t)n) {
        size_t new_sz = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp && new_sz > 0) abort();
        cache     = tmp;
        cache_sz  = new_sz;
    }

    /* fill initial values in the cache */
    if (cache_max < 13) {
        for (idx_t i = 1; i <= 13; ++i) cache[i] = init_vals[i];
        cache_max = 13;
    }

    /* compute values up to n */
    if (cache_max < n) {
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            seq_t pos = 0;
            seq_t neg = 0;
            for (int j = 1; j <= 12; ++j) {
                int c = coeff[j - 1];
                if (c > 0)
                    pos += (seq_t)c * cache[i - j];
                else
                    neg += (seq_t)(-c) * cache[i - j];
            }
            cache[i] = pos - neg;
        }
        cache_max = n;
    }

    return cache[n];
}
