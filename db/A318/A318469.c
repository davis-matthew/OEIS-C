/*
 * A318469  Multiplicative with a(p^e) = A019565(A003714(e)).
 * https://oeis.org/A318469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   multiplicative with a(p^e) = A019565(A003714(e)); a(1) = 1
 * Range:     u8 n <= 25919, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9735 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint32_t A318469_primes[64] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311
};

/* A003714: fibbinary number from Zeckendorf representation */
static uint64_t A318469_fibbinary(uint64_t rem) {
    uint64_t f[92];
    int k, i;
    uint64_t result = 0;
    f[0] = 1;
    f[1] = 2;
    k = 2;
    while (k < 92 && f[k - 1] <= rem) {
        f[k] = f[k - 1] + f[k - 2];
        k++;
    }
    for (i = k - 1; i >= 0; i--) {
        result = result * 2;
        if (f[i] <= rem) {
            rem -= f[i];
            result = result + 1;
        }
    }
    return result;
}

/* A019565(m) as a value of a(p^e) for exponent e */
static seq_t A318469_prime_power_value(uint64_t e) {
    uint64_t m = A318469_fibbinary(e);
    seq_t result = 1;
    int bit = 0;
    while (m != 0) {
        if (m & 1ULL) result *= (seq_t)A318469_primes[bit];
        m >>= 1;
        ++bit;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t result = 1;
    idx_t temp = n;
    uint64_t e;

    if (n <= 1) return 1;

    e = 0;
    while (temp % 2 == 0) { e++; temp /= 2; }
    if (e) result *= A318469_prime_power_value(e);

    for (idx_t i = 3; i <= temp / i; i += 2) {
        if (temp % i == 0) {
            e = 0;
            while (temp % i == 0) { e++; temp /= i; }
            result *= A318469_prime_power_value(e);
        }
    }
    if (temp > 1) result *= A318469_prime_power_value(1);
    return result;
}
