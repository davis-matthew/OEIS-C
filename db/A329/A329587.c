/*
 * A329587  Irregular triangle read by rows: representative solutions (a, b)
 *          of the complex congruence z^2 == +1 (mod m), where z = a + b*i =
 *          r*exp(i*phi), with nonvanishing a*b, for all positive integers m
 *          which have solutions.
 * https://oeis.org/A329587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is obtained by concatenating the a‑coordinates of all
 *            non‑trivial solutions (a,b) to the congruence z^2 ≡ +1 (mod m)
 *            where z = a + bi, 1 ≤ a,b < m, a·b ≠ 0, for every modulus m that
 *            has such solutions. The solutions for each m are listed first
 *            the pairs with a > b (sorted by increasing a) an...
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   gcd_u64, seq_append
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_u64.h"
#include "../utils/seq_append.h"

/* Euclidean gcd for uint64_t */

/* Pair of uint64_t used for temporary storage of solutions. */
struct pair64 { uint64_t a; uint64_t b; };

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static storage for generated values and the next modulus to test */
    static seq_t *seq = NULL;
    static size_t seq_cap = 0, seq_len = 0;
    static uint64_t m = 2;               /* next modulus to examine */

    while (seq_len < n) {
        /* collect pairs for the current modulus m */
        struct pair64 *left = NULL, *right = NULL;
        size_t left_cap = 0, left_len = 0;
        size_t right_cap = 0, right_len = 0;

        for (uint64_t a_val = 1; a_val < m; ++a_val) {
            uint64_t g2 = gcd_u64(2 * a_val, m);
            uint64_t step = m / g2;          /* minimal b step */
            uint64_t tmax = m / step;        /* number of b candidates + 1 */

            /* pre‑compute a^2 mod m */
            uint64_t a2 = (uint64_t)(((__uint128_t)a_val * a_val) % m);

            for (uint64_t k = 1; k < tmax; ++k) {
                uint64_t b_val = k * step;
                /* pre‑compute b^2 mod m */
                uint64_t b2 = (uint64_t)(((__uint128_t)b_val * b_val) % m);

                /* check a^2 - b^2 ≡ 1 (mod m) */
                if ((a2 + m - b2) % m != 1)
                    continue;
                /* 2*a*b ≡ 0 (mod m) holds by construction of step */

                if (a_val > b_val) {              /* left half */
                    if (left_len >= left_cap) {
                        left_cap = (left_cap == 0) ? 4 : (left_cap << 1);
                        left = (struct pair64 *)realloc(left,
                                           left_cap * sizeof(struct pair64));
                    }
                    left[left_len].a = a_val;
                    left[left_len].b = b_val;
                    left_len++;
                } else if (a_val < b_val) {        /* right half */
                    if (right_len >= right_cap) {
                        right_cap = (right_cap == 0) ? 4 : (right_cap << 1);
                        right = (struct pair64 *)realloc(right,
                                          right_cap * sizeof(struct pair64));
                    }
                    right[right_len].a = a_val;
                    right[right_len].b = b_val;
                    right_len++;
                }
                /* a == b never satisfies the congruence */
            }
        }

        /* store found pairs into the global sequence (a,b) order */
        for (size_t i = 0; i < left_len; ++i) {
            seq_append(&seq, &seq_len, &seq_cap,
                       (seq_t)left[i].a);
            seq_append(&seq, &seq_len, &seq_cap,
                       (seq_t)left[i].b);
        }
        for (size_t i = 0; i < right_len; ++i) {
            seq_append(&seq, &seq_len, &seq_cap,
                       (seq_t)right[i].a);
            seq_append(&seq, &seq_len, &seq_cap,
                       (seq_t)right[i].b);
        }

        /* free temporary pair arrays */
        free(left);
        free(right);

        /* move to next modulus */
        m++;
    }

    return seq[n - 1];
}
