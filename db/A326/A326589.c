/*
 * A326589  Sum of the smallest parts of the partitions of n into 10 parts.
 * https://oeis.org/A326589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{⌊n/10⌋} P_{≤10}(n-10k) where P_{≤10}(m) is the
 *            number of partitions of m whose parts are all ≤10
 * Range:     u8 n <= 26, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static bool precomputed = false;
#define MAX_N 20000
static seq_t part10[MAX_N + 1];

static void init_partitions(void)
{
    if (precomputed) return;
    for (idx_t i = 0; i <= MAX_N; ++i) part10[i] = 0;
    part10[0] = 1;
    for (idx_t coin = 1; coin <= 10; ++coin) {
        for (idx_t v = coin; v <= MAX_N; ++v) {
            part10[v] = part10[v] + part10[v - coin];
        }
    }
    precomputed = true;
}

seq_t a(idx_t n)
{
    init_partitions();
    idx_t t = n / 10;
    seq_t sum = 0;
    for (idx_t k = 1; k <= t; ++k) {
        idx_t idx = n - 10 * k;
        sum += part10[idx];
    }
    return sum;
}
