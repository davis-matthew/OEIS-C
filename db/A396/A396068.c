/*
 * A396068  Product of squarefree divisors of n that are <= sqrt(n).
 * https://oeis.org/A396068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = product of all square‑free divisors d of n with d ≤ sqrt(n)
 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Integer square root: floor(sqrt(n)) for 0 ≤ n ≤ UINT64_MAX */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = ((idx_t)1 << 32);  /* sqrt(UINT64_MAX) < 2^32 */
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (mid == 0) {          /* avoid division by zero */
            low = mid + 1;
            continue;
        }
        if (mid <= n / mid) {    /* mid*mid <= n  (avoids overflow) */
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;                 /* floor(sqrt(n)) */
}

seq_t a(idx_t n)
{
    /* floor(sqrt(n)) – used as a cut‑off for the divisors */
    idx_t sqrt_n = isqrt_u64(n);

    /* Factor n to collect its distinct prime factors */
    idx_t m = n;
    idx_t primes[64];
    size_t pcnt = 0;

    if (m % 2ULL == 0) {
        primes[pcnt++] = 2;
        while (m % 2ULL == 0) m /= 2;
    }
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            primes[pcnt++] = p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) {
        primes[pcnt++] = m;
    }

    /* Product of qualifying square‑free divisors */
    seq_t result = 1;

    if (pcnt > 0) {
        /* Enumerate all non‑empty subsets of the distinct primes */
        size_t subset_cnt = (size_t)1 << pcnt;   /* pcnt is ≤ 64 for n < 2^64 */
        for (size_t mask = 1; mask < subset_cnt; ++mask) {
            seq_t prod = 1;
            for (size_t i = 0; i < pcnt; ++i) {
                if (mask & ((size_t)1 << i)) {
                    prod *= (seq_t)primes[i];
                }
            }
            if (prod <= (seq_t)sqrt_n) {
                result *= prod;
            }
        }
    }

    return result;
}
