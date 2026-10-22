/*
 * A377282  Difference between n and the next prime-power (exclusive).
 * https://oeis.org/A377282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = the smallest prime power > n minus n (difference between
 *            n and the next prime‑power after n)
 * Range:     u8 n <= 12, u16 n <= 30, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   next_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/pow_uint.h"

/* Return ⌈n^(1/e)⌉ for n ≥ 1 and e ≥ 2; guaranteed that result^e > n. */
static uint64_t ceil_root_uint64(idx_t n, int e)
{
    if (n <= 1)        /* n = 1 → the smallest base is 2 */
        return 2;

    uint64_t lo = 1;
    uint64_t hi = n + 1;   /* (n+1)^e > n for any e ≥ 1 */
    seq_t N = (seq_t)n;

    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        seq_t pow_mid = pow_uint(mid, (idx_t)e);
        if (pow_mid > N)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)       /* offset is 1; behaviour below 1 is irrelevant */
        return 0;

    seq_t N = (seq_t)n;
    /* Exponent 1: the next prime > n (prime powers include primes). */
    seq_t best = (seq_t)(next_prime(n)) - N;
    if (best == 1)    /* cannot beat a difference of 1 */
        return best;

    /* Search exponents e ≥ 2 */
    for (int e = 2; ; ++e) {
        /* If even the minimum possible power 2^e exceeds the current best
           gap, further exponents cannot improve the result. */
        seq_t pow2e = pow_uint(2, (idx_t)e);
        if (pow2e > N + best)
            break;

        uint64_t base_min = ceil_root_uint64(n, e);
        /* Next prime ≥ base_min : next_prime(x) returns the first prime
           ≥ x (expanding the implementation to use next_prime(b-1) if
           the utility returns >b). */
        uint64_t p = next_prime(base_min - 1);
        seq_t val = pow_uint(p, (idx_t)e);
        seq_t diff = val - N;

        if (diff < best) {
            best = diff;
            if (best == 1)   /* optimal */
                break;
        }
    }
    return best;
}
