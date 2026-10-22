/*
 * A347820  Least positive integer k such that k*n is a balanced-parentheses
 *          number (A014486).
 * https://oeis.org/A347820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive k such that k*n is a balanced-parentheses
 *            number (A014486)
 * Range:     u8 n <= 30, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Compute bit length of a seq_t value. */
static unsigned bitlen_seq_t_custom(seq_t x) {
    unsigned len = 0;
    while (x) {
        x >>= 1;
        ++len;
    }
    return len;
}

/* Test whether x is a Dyck word (valid balanced-parentheses binary number). */
static bool is_balanced(seq_t x) {
    if (x == 0) return false;  /* must be non‑zero */

    unsigned len = bitlen_seq_t_custom(x);
    if (len & 1) return false;             /* odd length cannot be balanced */

    if ((x & 1) != 0) return false;        /* must end with 0 */

    int bal = 0;
    for (unsigned i = len; i > 0; --i) {
        if ((x >> (i - 1)) & 1) {
            ++bal;
        } else {
            --bal;
            if (bal < 0) return false;     /* too many closing brackets */
        }
    }
    return bal == 0;
}

/* Return the smallest k such that k*n is a balanced-parentheses number. */
seq_t a(idx_t n) {
    /* n is guaranteed to be ≥ 1. */
    if (n % 2 == 0) {                      /* n even → k can be odd or even, start at 1 */
        for (seq_t k = 1; ; ++k) {
            seq_t prod = (seq_t)n * k;
            if (is_balanced(prod)) return k;
        }
    } else {                               /* n odd → k must be even */
        for (seq_t k = 2; ; k += 2) {
            seq_t prod = (seq_t)n * k;
            if (is_balanced(prod)) return k;
        }
    }
}
