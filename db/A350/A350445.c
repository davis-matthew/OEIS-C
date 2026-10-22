/*
 * A350445  a(n) is the smallest number larger than a(n-1) that has only one
 *          digit in common with a(n-1).
 * https://oeis.org/A350445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Sequence A350445: a(1)=1; a(n) = smallest integer > a(n-1)
   having exactly one digit in common with a(n-1) and that digit
   occurs exactly once in each number. */

static void digit_frequencies(seq_t x, unsigned char cnt[10]) {
    /* Initialize counts */
    for (int i = 0; i < 10; ++i) cnt[i] = 0;
    if (x == (seq_t)0) {
        cnt[0] = 1;
        return;
    }
    while (x > (seq_t)0) {
        unsigned d = (unsigned)(x % (seq_t)10);
        cnt[d]++;
        x /= (seq_t)10;
    }
}

static bool has_exactly_one_common_digit(seq_t a, seq_t b) {
    unsigned char cnt_a[10], cnt_b[10];
    digit_frequencies(a, cnt_a);
    digit_frequencies(b, cnt_b);

    int common_digit = -1;         /* index of common digit, if any */
    for (int d = 0; d < 10; ++d) {
        if (cnt_a[d] && cnt_b[d]) {
            /* More than one common digit */
            if (common_digit != -1) return false;
            /* The common digit must appear exactly once in each number */
            if (cnt_a[d] != 1 || cnt_b[d] != 1) return false;
            common_digit = d;
        }
    }
    return common_digit != -1;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;

    seq_t prev = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t cand = prev + (seq_t)1;
        while (!has_exactly_one_common_digit(prev, cand))
            cand += (seq_t)1;
        prev = cand;
    }
    return prev;
}
