/*
 * A375078  a(1)=1; thereafter a(n) is the number of digits in (n mod 10)
 *          already present in the sequence.
 * https://oeis.org/A375078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=1; for n>1, a(n) = number of occurrences of the digit (n
 *            mod 10) in all digits of the sequence terms up to index n-1
 * Range:     u8 n <= 630, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static seq_t digit_counts[10] = {0};

static void add_term_digits(seq_t val) {
    if (val == 0) {
        digit_counts[0]++;   /* digit '0' appears once in number 0 */
        return;
    }
    seq_t t = val;
    while (t != 0) {
        seq_t d = t % 10;
        digit_counts[(size_t)d]++;
        t /= 10;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* safety for out‑of‑range */
    size_t required = (size_t)n;

    if (required > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 16;
        while (new_cap < required) new_cap <<= 1;

#ifdef __cplusplus
        seq_t *tmp = reinterpret_cast<seq_t *>(
            realloc(seq, new_cap * sizeof(seq_t)));
#else
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
#endif
        seq = tmp;
        seq_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = (idx_t)seq_len + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = 1;
        } else {
            idx_t d = i % 10;
            val = digit_counts[(size_t)d];
        }
        seq[seq_len++] = val;
        add_term_digits(val);
    }

    return seq[n - 1];
}
