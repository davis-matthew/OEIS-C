/*
 * A337636  Number of numbers k <= n such that k+1 is in A095096 and k is in
 *          A020899.
 * https://oeis.org/A337636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = #{ k <= n : Zeckendorf(k+1) has an even number of terms
 *            and Zeckendorf(k) has an odd number of terms }; computed by
 *            direct count
 * Range:     u8 n <= 923, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 95 terms through n = 9150 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t zk_fib[100];
static int zk_cnt = 0;

static void zk_init(void)
{
    if (zk_cnt) return;
    zk_fib[0] = 1;
    zk_fib[1] = 2;
    zk_cnt = 2;
    while (zk_cnt < 100) {
        uint64_t nx = zk_fib[zk_cnt - 1] + zk_fib[zk_cnt - 2];
        if (nx < zk_fib[zk_cnt - 1]) break; /* overflow */
        zk_fib[zk_cnt++] = nx;
    }
}

/* parity of number of Zeckendorf terms: 1 if odd, 0 if even */
static int zk_parity(uint64_t m)
{
    int p = 0;
    int i = zk_cnt - 1;
    while (m > 0) {
        while (zk_fib[i] > m) --i;
        m -= zk_fib[i];
        p ^= 1;
    }
    return p;
}

seq_t a(idx_t n)
{
    zk_init();
    uint64_t count = 0;
    uint64_t N = (uint64_t)n;
    int pk = 0; /* parity for k=0 */
    for (uint64_t k = 0; k <= N; ++k) {
        int pk1 = zk_parity(k + 1);
        if (pk == 1 && pk1 == 0) count++;
        pk = pk1;
    }
    return (seq_t)count;
}
