/*
 * A291750  Compound filter: a(n) = P(A003557(n), A048250(n)), where P(n,k) is
 *          sequence A000027 used as a pairing function.
 * https://oeis.org/A291750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2 + (x+y)^2 - x - 3*y)/2 with x = A003557(n), y =
 *            A048250(n)
 * Range:     u8 n <= 13, u16 n <= 185, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9447 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A003557_a(idx_t n) {
    seq_t result = 1;
    seq_t temp = (seq_t)n;
    for (seq_t p = 2; p * p <= temp; p++) {
        seq_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            e++;
        }
        for (seq_t i = 1; i < e; i++) {
            result *= p;
        }
    }
    return result;
}

static seq_t A048250_a(idx_t n) {
    if (n == 1)
        return (seq_t)1;
    uint64_t m = (uint64_t)n;
    seq_t result = (seq_t)1;
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            result = result * ((seq_t)p + 1);
            while (m % p == 0)
                m /= p;
        }
    }
    if (m > 1)
        result = result * ((seq_t)m + 1);
    return result;
}

seq_t a(idx_t n) {
    seq_t x = A003557_a(n);
    seq_t y = A048250_a(n);
    seq_t s = x + y;
    return (s * s + 2 - x - 3 * y) / 2;
}
