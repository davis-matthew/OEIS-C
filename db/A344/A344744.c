/*
 * A344744  a(n) is the n-th power of the concatenation of the integers from 0
 *          through n-1.
 * https://oeis.org/A344744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007908(n-1)^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A007908(k) is the decimal concatenation of integers 0..k */

static seq_t pow10_seq(idx_t exp) {
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i) {
        result *= 10;
    }
    return result;
}

static unsigned digits_idx(idx_t x) {
    unsigned count = 0;
    if (x == 0) return 1;
    while (x > 0) {
        ++count;
        x /= 10;
    }
    return count;
}

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t concat = 0;
    for (idx_t i = 0; i < n; ++i) {
        unsigned d = digits_idx(i);
        seq_t mult = pow10_seq(d);
        concat = concat * mult + (seq_t)i;
    }
    return pow_seq(concat, n);
}
