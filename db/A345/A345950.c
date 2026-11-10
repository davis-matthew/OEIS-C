/*
 * A345950  a(n) = 1 if for all proper divisors d of n, A002034(d) <
 *          A002034(n), otherwise 0.
 * https://oeis.org/A345950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A002034(d) < A002034(n) for all proper divisors d of
 *            n, else 0; since A002034 is monotone along divisibility, it
 *            suffices to test d = n/p for each prime p | n
 * Range:     u8 n <= 767, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 136 terms through n = 9208 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static seq_t A345950_kemp(seq_t n) {
    if (n <= 1) return 1;
    seq_t result = 1;
    seq_t temp = n;
    seq_t p;
    for (p = 2; p <= temp / p; p++) {
        if (temp % p == 0) {
            seq_t k = 0;
            while (temp % p == 0) { temp /= p; k++; }
            seq_t cnt = 0, m = 0;
            while (cnt < k) {
                m += p;
                seq_t t = m;
                while (t % p == 0) { t /= p; cnt++; }
            }
            if (m > result) result = m;
        }
    }
    if (temp > 1 && temp > result) result = temp;
    return result;
}

seq_t a(idx_t n_idx) {
    seq_t n = (seq_t)n_idx;
    if (n <= 1) return 1;
    seq_t kn = A345950_kemp(n);
    seq_t temp = n;
    seq_t p;
    for (p = 2; p <= temp / p; p++) {
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            if (A345950_kemp(n / p) == kn) return 0;
        }
    }
    if (temp > 1) {
        if (A345950_kemp(n / temp) == kn) return 0;
    }
    return 1;
}
