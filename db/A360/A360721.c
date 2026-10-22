/*
 * A360721  a(n) is the number of infinitary divisors of n that are powerful
 *          (A001694).
 * https://oeis.org/A360721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e‖n} (2^{popcount(e)} - (e mod 2))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiply 2 k times, works for all concrete seq_t sizes */
static inline seq_t pow2_small(unsigned int k)
{
    seq_t r = (seq_t)1;
    while (k--) r *= (seq_t)2;
    return r;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;          /* defined for n≥1 */
    uint64_t temp = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* handle factor 2 */
    unsigned int exp = 0;
    while ((temp & 1ULL) == 0ULL) {
        temp >>= 1;
        exp++;
    }
    if (exp != 0) {
        unsigned int pc = __builtin_popcountll(exp);
        seq_t val = pow2_small(pc);
        if (exp & 1U) val -= (seq_t)1;
        result *= val;
    }

    /* handle odd prime factors */
    for (uint64_t p = 3ULL; p <= temp / p; p += 2U) {
        if (temp % p != 0ULL) continue;
        exp = 0;
        while (temp % p == 0ULL) {
            temp /= p;
            exp++;
        }
        unsigned int pc = __builtin_popcountll(exp);
        seq_t val = pow2_small(pc);
        if (exp & 1U) val -= (seq_t)1;
        result *= val;
    }

    /* remaining prime factor (if any) has exponent 1, contributes (2¹-1)=1 */
    return result;
}
