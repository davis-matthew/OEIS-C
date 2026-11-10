/*
 * A288575  Partial sums of A104324.
 * https://oeis.org/A288575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A104324(k), where A104324(k) = number of
 *            runs of identical bits in the Zeckendorf representation of k
 * Range:     u8 n <= 58, u16 n <= 6674, u32 n <= 28656, u64 n <= 28656,
 *            u128 n <= 28656, big any n
 * Validated: standard, pass, 83 terms through n = 9530 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A288575_fib[94];
static bool A288575_inited = false;

static void A288575_init(void)
{
    if (A288575_inited) return;
    A288575_fib[1] = 1ULL;
    A288575_fib[2] = 2ULL;
    for (int i = 3; i <= 93; ++i) A288575_fib[i] = A288575_fib[i-1] + A288575_fib[i-2];
    A288575_inited = true;
}

static uint64_t A288575_runs(uint64_t n)
{
    if (n == 0ULL) return 0;
    int idx = 93;
    while (A288575_fib[idx] > n) --idx;
    uint64_t runs = 0;
    bool prev_bit = false;
    bool started = false;
    uint64_t m = n;
    for (int i = idx; i >= 1; --i) {
        bool bit = (m >= A288575_fib[i]);
        if (bit) m -= A288575_fib[i];
        if (!started || bit != prev_bit) {
            ++runs;
            started = true;
        }
        prev_bit = bit;
    }
    return runs;
}

seq_t a(idx_t n)
{
    A288575_init();
    seq_t total = 0;
    for (uint64_t k = 1; k <= (uint64_t)n; ++k) {
        total += (seq_t)A288575_runs(k);
    }
    return total;
}
