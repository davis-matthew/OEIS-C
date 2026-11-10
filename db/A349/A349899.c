/*
 * A349899  Least number in A349898 divisible by the n-th prime.
 * https://oeis.org/A349899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, more
 * Formula:   a(n) = f(p) with p = prime(n), where f(0) = 1 and f(s) = s *
 *            min(f(s-9..s-1), terms with index >= 0). f(s) is the least
 *            product of prefix digit sums that ends at total digit sum s; the
 *            code computes it by DP
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 16 (2026-11-10)
 * Helpers:   mem, nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/nth_prime.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    uint64_t p = nth_prime(n);
    seq_t *f = mem_new(seq_t, (size_t)p + 1);
    f[0] = (seq_t)1;
    for (uint64_t s = 1; s <= p; s++) {
        uint64_t lo = (s >= 9) ? s - 9 : 0;
        seq_t best = f[lo];
        for (uint64_t t = lo + 1; t < s; t++)
            if (f[t] < best) best = f[t];
        f[s] = best * (seq_t)s;
    }
    seq_t r = f[p];
    mem_free(f);
    return r;
}
