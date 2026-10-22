/*
 * A381973  Numbers m such that Sum_{k >= 0} floor(m/3^k) is prime.
 * https://oeis.org/A381973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 3508, u32 n <= 3938, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, sum_floor_div3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: Numbers m such that Sum_{k >= 0} floor(m / 3^k) is prime. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/sum_floor_div3.h"

/* Return the n‑th term of A381973 (1‑based index) */
seq_t a(idx_t n) {
    static seq_t *seq = NULL;
    static size_t len = 0;
    static size_t cap = 0;
    static seq_t next_m = 2;

    if (len >= (size_t)n) {
        return seq[(size_t)(n - 1)];
    }

    if (cap == 0) {
        cap = 16;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    while (len < (size_t)n) {
        seq_t m = next_m;
        seq_t s = sum_floor_div3(m);
        if (is_prime_big(s)) {
            if (len >= cap) {
                cap <<= 1;
                seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
            }
            seq[len++] = m;
        }
        next_m++;
    }

    return seq[(size_t)(n - 1)];
}
