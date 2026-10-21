/*
 * A277859  Least k > 1 such that 1^(k-1) + 2^(k-1) + 3^(k-1) + ... +
 *          (k-1)^(k-1) - n == 0 (mod k).
 * https://oeis.org/A277859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least k > 1 such that 1^(k-1) + 2^(k-1) + ... +
 *            (k-1)^(k-1) ≡ n (mod k) (offset 1)
 * Range:     u8 n <= 261, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* fast modular exponentiation: base^exp mod mod   (mod <= 2^32-1) */
static seq_t powmod(seq_t base, idx_t exp, seq_t mod)
{
    /* base and mod are small (≤ a few 10⁴) so casting to uint64_t is safe */
    uint64_t b = (uint64_t)(base % mod);
    uint64_t m = (uint64_t)mod;
    uint64_t r = 1 % m;
    while (exp) {
        if (exp & 1ULL) r = (r * b) % m;
        b = (b * b) % m;
        exp >>= 1ULL;
    }
    return (seq_t)r;
}

seq_t a(idx_t n)
{
    /* odd n always satisfy k = 2 */
    if ((n & 1ULL) == 1ULL)
        return (seq_t)2;

    /* search for the smallest k > 1 */
    for (idx_t k = 3; ; ++k) {
        seq_t k_seq = (seq_t)k;
        seq_t sum = 0;
        for (idx_t i = 1; i < k; ++i) {
            sum += powmod((seq_t)i, k - 1, k_seq);
            /* keep sum < k_seq to avoid overflow */
            if (sum >= k_seq) sum -= k_seq;
        }
        if ((sum % k_seq) == (seq_t)(n % k))
            return k_seq;
    }
}
