/*
 * A320584  Numbers whose first digit is prime.
 * https://oeis.org/A320584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth positive integer whose first decimal digit is 2, 3, 5
 *            or 7
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const idx_t pow10[20] = {
    1ULL,
    10ULL,
    100ULL,
    1000ULL,
    10000ULL,
    100000ULL,
    1000000ULL,
    10000000ULL,
    100000000ULL,
    1000000000ULL,
    10000000000ULL,
    100000000000ULL,
    1000000000000ULL,
    10000000000000ULL,
    100000000000000ULL,
    1000000000000000ULL,
    10000000000000000ULL,
    100000000000000000ULL,
    1000000000000000000ULL,
    10000000000000000000ULL
};

static idx_t count_leq(idx_t x) {
    if (x == 0) return 0;
    /* count digit length */
    unsigned d = 1;
    idx_t tmp = x;
    while (tmp >= 10) {
        tmp /= 10;
        ++d;
    }

    /* numbers with fewer digits */
    idx_t total = 0;
    for (unsigned len = 1; len < d; ++len) {
        total += 4ULL * pow10[len - 1];
    }

    /* numbers with the same length */
    idx_t firstdigit = x / pow10[d - 1];
    idx_t rest = x % pow10[d - 1];
    const unsigned primes[4] = {2, 3, 5, 7};
    for (unsigned k = 0; k < 4; ++k) {
        if (primes[k] < firstdigit) {
            total += pow10[d - 1];
        }
    }
    if (firstdigit == 2 || firstdigit == 3 || firstdigit == 5 || firstdigit == 7) {
        total += rest + 1;
    }
    return total;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n=0, but safe */

    idx_t low = 1;
    idx_t high = n * 10ULL;
    if (high == 0) high = 1;  /* wrap‑around protection, though impossible for n>=1 */

    /* ensure high large enough */
    while (count_leq(high) < n) {
        if (high > UINT64_MAX / 2) { /* avoid overflow */
            high = UINT64_MAX;
            break;
        }
        high <<= 1;
    }

    /* binary search */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (count_leq(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return (seq_t)low;
}
