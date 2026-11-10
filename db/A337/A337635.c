/*
 * A337635  Number of numbers k <= n such that k is in A095096 and k+1 is in
 *          A020899.
 * https://oeis.org/A337635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = #{k in 0..n : Zeckendorf(k) has an even number of terms
 *            and Zeckendorf(k+1) has an odd number}; computed by direct scan
 * Range:     u8 n <= 922, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 8636 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A337635_fib[92];
static int A337635_nf = 0;

static void A337635_init(void)
{
    if (A337635_nf) return;
    A337635_fib[0] = 1;
    A337635_fib[1] = 2;
    A337635_nf = 2;
    while (A337635_nf < 92) {
        uint64_t nx = A337635_fib[A337635_nf - 1] + A337635_fib[A337635_nf - 2];
        if (nx < A337635_fib[A337635_nf - 1]) break;
        A337635_fib[A337635_nf++] = nx;
    }
}

/* returns parity (0 even, 1 odd) of number of Zeckendorf terms */
static int A337635_par(uint64_t k)
{
    int p = 0;
    int i = A337635_nf - 1;
    while (k > 0) {
        while (A337635_fib[i] > k) --i;
        k -= A337635_fib[i];
        p ^= 1;
    }
    return p;
}

seq_t a(idx_t n)
{
    A337635_init();
    uint64_t cnt = 0;
    int pk = 0; /* parity of 0 */
    for (uint64_t k = 0; k <= (uint64_t)n; ++k) {
        int pk1 = A337635_par(k + 1);
        if (pk == 0 && pk1 == 1) ++cnt;
        pk = pk1;
    }
    return (seq_t)cnt;
}
