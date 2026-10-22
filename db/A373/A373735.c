/*
 * A373735  a(n) = p^floor(log_p n) for p = A020639(n).
 * https://oeis.org/A373735
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p^floor(log_p n) for p = A020639(n)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline uint64_t _lpf(idx_t n) {
    if (n % 2 == 0) return 2;
    for (idx_t i = 3; i <= n / i; i += 2) {
        if (n % i == 0) return i;
    }
    return n;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;                      /* undefined for n < 2 */
    uint64_t p = _lpf(n);                     /* small prime factor */
    seq_t p_seq = (seq_t)p;                   /* promote to seq_t */
    seq_t result = 1;
    seq_t limit = ((seq_t)n) / p_seq;         /* largest power base <= n */
    while (result <= limit) {
        result *= p_seq;
    }
    return result;
}
