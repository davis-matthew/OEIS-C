/*
 * A243361  a(n) = arrange digits of concatenation of divisors of n (A037278,
 *          A176558) in increasing order in base 10 (zero digits are omitted).
 * https://oeis.org/A243361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = arrange digits of concatenation of every divisor of n (in
 *            base 10) in increasing order, with all zero digits omitted
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 35, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count decimal digits of a divisor other than zero. */
static inline void add_digits(uint64_t d, unsigned char cnt[10]) {
    if (d == 0) return;   /* divisors never zero, but guard anyway */
    while (d != 0) {
        unsigned digit = (unsigned)(d % 10);
        if (digit != 0) { cnt[digit]++; }
        d /= 10;
    }
}

seq_t a(idx_t n) {
    /* avoid underflow for n == 0: behaviour below offset is irrelevant */
    if (n == 0) return 0;

    uint64_t n64 = (uint64_t)n;          /* local copy for arithmetic */
    unsigned char cnt[10] = {0};

    /* enumerate divisors */
    for (uint64_t d = 1; d * d <= n64; ++d) {
        if (n64 % d == 0) {
            add_digits(d, cnt);
            uint64_t e = n64 / d;
            if (e != d) add_digits(e, cnt);
        }
    }

    /* build resulting number by concatenating digits in ascending order */
    seq_t res = 0;
    for (unsigned d = 1; d <= 9; ++d) {
        for (unsigned i = 0; i < cnt[d]; ++i) {
            res = res * 10 + d;
        }
    }
    return res;
}
