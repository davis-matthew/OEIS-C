/*
 * A343650  a(n) is the number of divisors d of n such that the product d *
 *          (n/d) can be computed without carries in binary.
 * https://oeis.org/A343650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 6965 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) : number of divisors d of n such that d*(n/d) can be
 * multiplied in binary without any carry occurring. */

#include <stdint.h>
#include <stdbool.h>

/* Helper: test whether binary multiplication of a and b has
 * no carries (i.e., each bit position of a+b sum <= 1). */
static bool carry_free(uint64_t a, uint64_t b)
{
    unsigned char seen[128] = {0};
    for (int i = 0; a >> i; ++i)
        if ((a >> i) & 1) {
            for (int j = 0; b >> j; ++j)
                if ((b >> j) & 1) {
                    int s = i + j;
                    if (seen[s])
                        return false;
                    seen[s] = 1;
                }
        }
    return true;
}

seq_t a(idx_t n)
{
    /* n is 1-based; behaviour for n < 1 is irrelevant */
    uint64_t count = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            uint64_t d = i;
            uint64_t e = n / i;
            bool ok = carry_free((uint64_t)d, (uint64_t)e);
            count += ok ? (d == e ? 1 : 2) : 0;
        }
    }
    return (seq_t)count;
}
