/*
 * A296843  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n+1), where a(0) = 1, a(1) = 2, b(0) = 3, b(1) = 4, b(2) = 5,
 *          b(3) = 6, and (a(n)) and (b(n)) are increasing complementary
 *          sequences.
 * https://oeis.org/A296843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n+1) a(0)=1, a(1)=2 Complementary
 *            increasing sequences with b(0)=3, b(1)=4, b(2)=5, b(3)=6
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for a and b sequences */
    static seq_t *a_vals = NULL;
    static size_t a_size = 0, a_cap = 0;

    static seq_t *b_vals = NULL;
    static size_t b_size = 0, b_cap = 0;

    /* next integer to test for b and index into a for comparison */
    static seq_t next_b_candidate = (seq_t)3;
    static size_t a_pos = 0;

    /* generate a(n) if needed */
    while (a_size <= (size_t)n) {
        size_t a_cur = a_size;

        seq_t new_a;
        if (a_cur == 0) {
            new_a = (seq_t)1;
        } else if (a_cur == 1) {
            new_a = (seq_t)2;
        } else {
            /* ensure b has entry at index a_cur+1 */
            size_t need_b = a_cur + 1;
            while (b_size <= need_b) {
                /* find next integer not in a */
                seq_t cand = next_b_candidate;

                while (a_pos < a_size && a_vals[a_pos] < cand) {
                    ++a_pos;
                }

                if (a_pos < a_size && a_vals[a_pos] == cand) {
                    /* cand is in a, skip it */
                    ++next_b_candidate;
                    continue;
                }

                /* cand is not in a: store it as next b value */
                if (b_size == b_cap) {
                    size_t new_cap = (b_cap == 0) ? 4 : b_cap * 2;
                    b_vals = (seq_t *)realloc(b_vals, new_cap * sizeof(seq_t));
                    b_cap = new_cap;
                }
                b_vals[b_size++] = cand;
                ++next_b_candidate;
            }

            new_a = a_vals[a_cur - 1] + a_vals[a_cur - 2] + b_vals[a_cur + 1];
        }

        /* store new a value */
        if (a_size == a_cap) {
            size_t new_cap = (a_cap == 0) ? 4 : a_cap * 2;
            a_vals = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
            a_cap = new_cap;
        }
        a_vals[a_size++] = new_a;
    }

    return a_vals[(size_t)n];
}
