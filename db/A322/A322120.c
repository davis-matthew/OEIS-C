/*
 * A322120  a(n) is the smallest composite k such that n^(k-1) == 1 (mod
 *          (n^2-1)*k).
 * https://oeis.org/A322120
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest composite k such that n^(k-1) == 1 (mod
 *            (n^2-1)*k)
 * Range:     u8 none, u16 n <= 13, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime_small, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/mod_pow.h"
#include "../utils/is_prime_small.h"

static inline bool is_composite(seq_t x) {
    return x >= (seq_t)4 && !is_prime_small(x);
}

seq_t a(idx_t n) {
    if (n < (idx_t)2) return (seq_t)0;   /* undefined for offset <2 */

    seq_t n_seq = (seq_t)n;
    seq_t n_sq  = n_seq * n_seq;         /* n^2 */
    seq_t k = (seq_t)4;                  /* start at smallest composite */

    for (;;) {
        if (is_composite(k)) {
            seq_t mod = (n_sq - (seq_t)1) * k;          /* (n^2-1)*k */
            seq_t r = mod_pow(n_seq, (idx_t)(k - (seq_t)1), mod);
            if (r == (seq_t)1) return k;
        }
        ++k;
    }
}
