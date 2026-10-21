/*
 * A273283  Least prime not less than the geometric mean of all prime divisors
 *          of n counted with multiplicity.
 * https://oeis.org/A273283
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime ≥ n^(1/ω(n)), where ω(n) is the number of *
 *            prime factors of n counted with multiplicity
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 1151, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/_smallest_prime_factor.h"
#include "../utils/is_prime_big.h"
#include "../utils/next_prime.h"

/* exponentiation by squaring: returns base^exp */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* integer ceil k‑th root of N: smallest r such that r^k ≥ N.
 * N ≤ 2^64−1, so we can keep low/high in idx_t.
 */
static idx_t ceil_nth_root(seq_t N, idx_t k)
{
    if (k == 1) return (idx_t)N;
    idx_t low = 1;
    idx_t high = (idx_t)N;               /* N fits into idx_t */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        seq_t mid_pow = pow_seq((seq_t)mid, k);
        if (mid_pow < N)
            low = mid + 1;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* undefined for n<2 */

    /* compute ω(n) */
    idx_t omega = 0;
    idx_t rem = n;
    while (rem > 1) {
        uint64_t p = _smallest_prime_factor(rem);
        do {
            rem /= p;
            ++omega;
        } while (rem % p == 0);
    }

    /* ceil root */
    idx_t r = ceil_nth_root((seq_t)n, omega);
    if (r < 2) r = 2;                     /* safety for n=2 */

    /* find least prime ≥ r */
    if (is_prime_big((seq_t)r))
        return (seq_t)r;
    return (seq_t)next_prime(r);
}
