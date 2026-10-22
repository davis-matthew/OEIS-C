/*
 * A330607  Array read by rows: T(n,k) is the number of solutions to the
 *          equation Sum_{i=1..n} x_i^2 == k (mod 5) with x_i in 0..4, where n
 *          >= 0 and 0 <= k <= 4.
 * https://oeis.org/A330607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = T(⌊n/5⌋, n mod 5), where T(n,k) counts solutions of
 *            x1²+…+xn² ≡ k (mod 5), xi∈{0,…,4}. Recurrence for n ≥ 2: T(0,k)
 *            = [1,0,0,0,0] T(1,k) = [1,2,0,0,2] T(2,k) = [9,4,4,4,4] T(n,k) =
 *            5·T(n–2,k) + 4·5^(n–2) (0 ≤ k ≤ 4)
 * Range:     u8 n <= 24, u16 n <= 39, u32 n <= 74, u64 n <= 144,
 *            u128 n <= 284, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static caches for the five sequences T(·,k) */
static seq_t *tcache[5] = {0};
static idx_t  tlen[5]  = {0};
static idx_t  tcap[5]  = {0};
static idx_t  maxm     = 0;

/* cache of 5^i */
static seq_t *pow5    = NULL;
static idx_t  pow5len = 0;
static idx_t  pow5cap = 0;

/* initialization flag */
static int initialized = 0;

static void init_cache(void)
{
    const seq_t init0[5] = {1,0,0,0,0};
    const seq_t init1[5] = {1,2,0,0,2};
    const seq_t init2[5] = {9,4,4,4,4};

    for (idx_t k = 0; k < 5; ++k) {
        tcap[k] = 4;                    /* initial capacity */
        tlen[k] = 3;                    /* indices 0,1,2 are known */
        tcache[k] = (seq_t *)malloc(tcap[k] * sizeof(seq_t));
        tcache[k][0] = init0[k];
        tcache[k][1] = init1[k];
        tcache[k][2] = init2[k];
    }

    pow5cap = 4;
    pow5    = (seq_t *)malloc(pow5cap * sizeof(seq_t));
    pow5[0] = (seq_t)1;
    pow5len = 1;

    maxm = 2;                          /* we have values for n = 0,1,2 */
    initialized = 1;
}

/* extend all caches so that each tcache[k][0..m] is defined   */
static void extend_cache(idx_t m)
{
    if (m <= maxm) return;   /* already available */

    /* enlarge pow5 array if needed */
    if (pow5cap < m + 1) {
        idx_t newcap = pow5cap ? pow5cap : 4;
        while (newcap <= m) newcap <<= 1;
        pow5 = (seq_t *)realloc(pow5, newcap * sizeof(seq_t));
        pow5cap = newcap;
    }
    for (idx_t i = pow5len; i <= m; ++i) {
        pow5[i] = pow5[i-1] * (seq_t)5;
    }
    pow5len = m + 1;

    /* enlarge and compute each T(·,k) cache */
    for (idx_t k = 0; k < 5; ++k) {
        if (tcap[k] < m + 1) {
            idx_t newcap = tcap[k] ? tcap[k] : 4;
            while (newcap <= m) newcap <<= 1;
            tcache[k] = (seq_t *)realloc(tcache[k], newcap * sizeof(seq_t));
            tcap[k] = newcap;
        }
        for (idx_t i = tlen[k]; i <= m; ++i) {
            if (i < 3) continue;                    /* base values already set */
            seq_t val = 5 * tcache[k][i-2] + 4 * pow5[i-2];
            tcache[k][i] = val;
        }
        tlen[k] = m + 1;
    }
    maxm = m;
}

seq_t a(idx_t n)
{
    if (!initialized) init_cache();

    idx_t m = n / 5;          /* floor division */
    idx_t k = n % 5;          /* remainder */

    extend_cache(m);
    return tcache[k][m];
}
