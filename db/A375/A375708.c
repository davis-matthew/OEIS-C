/*
 * A375708  First differences of non-prime-powers (exclusive, so 1 is not a
 *          prime-power).
 * https://oeis.org/A375708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = difference between the (n+1)th and nth non-prime-powers
 *            (exclusive). The sequence A375708 is computed directly by
 *            scanning integers and testing each for being a prime power
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Helper: true iff x is a prime power (p^k, p prime, k≥1); 1 is false. */
static bool is_prime_power(uint64_t x)
{
    if (x <= 1) return false;
    uint64_t d = 0;
    for (uint64_t i = 2; i <= x / i; ++i) {
        if (x % i == 0) { d = i; break; }
    }
    if (d == 0) /* x is prime itself */
        return true;
    /* divide out all factors d */
    while (x % d == 0) x /= d;
    return x == 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out‑of‑range, if ever called */
    uint64_t last_nonprime = 1;           /* first element of A361102 */
    uint64_t candidate = 2;               /* start scanning after 1 */
    idx_t diff_count = 0;                 /* number of differences found */

    while (diff_count < n) {
        /* search for next non‑prime‑power */
        while (true) {
            if (!is_prime_power(candidate)) break;
            ++candidate;
        }
        /* candidate is next non‑prime‑power */
        seq_t diff = (seq_t)(candidate - last_nonprime);
        ++diff_count;
        if (diff_count == n) return diff;
        last_nonprime = candidate;
        ++candidate;
    }
    /* unreachable, but keep compiler happy */
    return (seq_t)0;
}
