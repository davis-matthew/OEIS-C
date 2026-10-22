/*
 * A379457  If n = Product (p_j^k_j) then a(n) = Product (floor(p_j^(1/k_j))).
 * https://oeis.org/A379457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Product over prime factors p^k of n of floor(p^(1/k))
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function directly factors n using trial division and computes the
   required product.  No external libraries are used beyond standard C. */

#include <stdint.h>
#include <stddef.h>

static int pow_leq(uint64_t base, uint64_t exp, uint64_t limit)
{
    uint64_t r = 1;
    while (exp--) {
        if (base == 0) return 1;
        if (r > limit / base) return 0;
        r *= base;
    }
    return r <= limit;
}

static uint64_t kth_root(uint64_t p, uint64_t k)
{
    if (k == 1) return p;
    uint64_t low = 1, high = p, ans = 1;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        if (pow_leq(mid, k, p)) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* n is zero‑based indexed in the build; the sequence starts at 1. */
    uint64_t m = (uint64_t)n;
    if (m <= 1) return 1;        /* a(1) = 1 */

    seq_t result = 1;
    uint64_t temp = m;

    /* Handle factor 2 */
    if ((temp & 1) == 0) {
        uint64_t k = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            k++;
        }
        uint64_t r = kth_root(2, k);
        result *= (seq_t)r;
    }

    /* Odd factors */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            uint64_t k = 0;
            while (temp % p == 0) {
                temp /= p;
                k++;
            }
            uint64_t r = kth_root(p, k);
            result *= (seq_t)r;
        }
    }

    /* Remaining prime factor */
    if (temp > 1) {
        uint64_t r = kth_root(temp, 1);
        result *= (seq_t)r;
    }

    return result;
}
