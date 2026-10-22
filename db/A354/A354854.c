/*
 * A354854  Smallest number of powers of n with positive exponents that sum to
 *          a factorial.
 * https://oeis.org/A354854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = min_{m ≥ A002034(n)} digit_sum_base_n(m!)
 * Range:     u8 n <= 28, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A002034(n) is the smallest integer m such that n | m! */

#include <stdlib.h>
#include <stdint.h>

/* exponent of prime p in m! (Legendre's formula) */
static idx_t prime_exp_in_factorial(idx_t m, idx_t p)
{
    idx_t exp = 0;
    while (m >= p) {
        m /= p;
        exp += m;
    }
    return exp;
}

/* minimal m such that the exponent of p in m! is at least e */
static idx_t minimal_m_for_prime(idx_t p, idx_t e)
{
    if (e == 0) return 1;
    idx_t low = 1;
    idx_t high = p * e;
    if (high == 0) high = 1;          /* overflow guard */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (prime_exp_in_factorial(mid, p) >= e)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;          /* offset handling */

    /* --------- find n₀ = A002034(n) --------- */
    idx_t m0 = 1;
    idx_t tmp = n;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            idx_t m_p = minimal_m_for_prime(p, e);
            if (m_p > m0) m0 = m_p;
        }
    }
    if (tmp > 1) {                /* remaining prime factor */
        idx_t p = tmp;
        idx_t m_p = minimal_m_for_prime(p, 1);
        if (m_p > m0) m0 = m_p;
    }

    /* search up to m₀ + n for a minimal digit sum */
    idx_t limit = m0 + n;

    /* factorial digits in base n (stored as idx_t for safety) */
    size_t cap = 64;
    idx_t *digits = (idx_t*)malloc(cap * sizeof(idx_t));
    if (!digits) return 0;          /* allocation failure */
    digits[0] = 1;                  /* 1! = 1 */
    size_t len = 1;

    uint64_t best = UINT64_MAX;

    for (idx_t m = 2; m <= limit; ++m) {
        idx_t carry = 0;
        for (size_t i = 0; i < len; ++i) {
            idx_t prod = digits[i] * m + carry;
            digits[i] = prod % n;
            carry = prod / n;
        }
        while (carry) {
            if (len == cap) {
                cap <<= 1;
                idx_t *tmp = (idx_t*)realloc(digits, cap * sizeof(idx_t));
                if (!tmp) { free(digits); return 0; }
                digits = tmp;
            }
            digits[len] = carry % n;
            carry /= n;
            ++len;
        }

        if (m >= m0) {
            uint64_t sum = 0;
            for (size_t i = 0; i < len; ++i) sum += digits[i];
            if (sum < best) best = sum;
        }
    }

    free(digits);
    return (seq_t)best;
}
