/*
 * A340491  Number of n-digit numbers x such that rev(x^2) = rev(x)^2 and x
 *          does not contain any zero digits, where rev(x) is the digit
 *          reversal of x.
 * https://oeis.org/A340491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = number of n-digit numbers with no zero digits such that
 *            rev(x^2) = rev(x)^2
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t rev_uint64(uint64_t n) {
    uint64_t r = 0;
    while (n) {
        r = r * 10 + n % 10;
        n /= 10;
    }
    return r;
}

static void gen_helper(idx_t pos, idx_t len, uint64_t val, uint64_t rev_val,
                       uint64_t pow10, seq_t *cnt) {
    if (pos == len) {
        uint64_t x     = val;
        uint64_t revx  = rev_val;
        uint64_t sq    = x * x;           /* x^2 <= 10^18, fits in 64 bits */
        uint64_t rev_sq  = rev_uint64(sq);
        uint64_t revx_sq = revx * revx;   /* also fits in 64 bits */
        if (rev_sq == revx_sq) (*cnt)++;
        return;
    }
    /* digits allowed: 1, 2, 3 only (no zeros) */
    for (uint64_t d = 1; d <= 3; ++d) {
        uint64_t new_val  = val * 10 + d;
        uint64_t new_rev  = d * pow10 + rev_val;
        gen_helper(pos + 1, len, new_val, new_rev, pow10 * 10, cnt);
    }
}

seq_t a(idx_t n) {
    if (n > 9) return 0;                /* no solutions beyond 9 digits */
    static seq_t cache[10] = {0};
    static int computed[10] = {0};

    if (computed[n]) return cache[n];

    seq_t count = 0;
    gen_helper(0, n, 0, 0, 1, &count);

    cache[n] = count;
    computed[n] = 1;
    return count;
}
