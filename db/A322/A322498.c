/*
 * A322498  Number of tilings of a 5 X n rectangle using V (2m+1)-ominoes (m
 *          >= 0) in standard orientation.
 * https://oeis.org/A322498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A322498 (tilings of 5 × n by V‑(2m+1)-ominoes)
 *
 *  The sequence satisfies a linear recurrence of order 16:
 *
 *      a(n) = 1·a(n‑1) + 10·a(n‑2) + 36·a(n‑3) + 93·a(n‑4) +
 *             240·a(n‑5)+392·a(n‑6)+551·a(n‑7)+576·a(n‑8)+
 *             361·a(n‑9)+324·a(n‑10)+58·a(n‑11)+58·a(n‑12)+57·a(n‑13)
 *           - 76·a(n‑14) + 50·a(n‑15) – 10·a(n‑16)
 *
 *  Initial values (n = 0 … 15):
 *
 *      1, 1, 8, 44, 233, 1262, 6523, 34468, 181615,
 *      957006, 5044388, 26575335, 140039124, 737911089,
 *      3888300180, 20488828781
 *
 *  The function a(idx_t n) computes a(n) for any n ≥ 0 using the
 *  recurrence.  The calculation is O(n) with a very small constant
 *  factor (16 additions and multiplications per term).
 *
 *  The implementation uses only the operators +, -, * and indexing
 *  on the type seq_t, so it works uniformly for any unsigned
 *  integral type or arbitrary‑precision integer type supplied in the
 *  build system.
 */

#include <stddef.h>  /* for size_t */

static const int coeff[16] = {
     1,   10,   36,   93,  240,  392,  551,  576,
   361,  324,   58,   58,   57,  -76,   50,  -10
};

static const seq_t init[16] = {
   (seq_t)1ULL,
   (seq_t)1ULL,
   (seq_t)8ULL,
   (seq_t)44ULL,
   (seq_t)233ULL,
   (seq_t)1262ULL,
   (seq_t)6523ULL,
   (seq_t)34468ULL,
   (seq_t)181615ULL,
   (seq_t)957006ULL,
   (seq_t)5044388ULL,
   (seq_t)26575335ULL,
   (seq_t)140039124ULL,
   (seq_t)737911089ULL,
   (seq_t)3888300180UL,
   (seq_t)20488828781UL
};

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n < 16)
        return init[(size_t)n];

    /* sliding buffer holding the last 16 values; buffer[i] corresponds to a(i) */
    seq_t buf[16];
    size_t pos = 0;                /* index of a(n‑1) in buf */
    for (size_t i = 0; i < 16; ++i)
        buf[i] = init[i];
    pos = 15;                      /* a(15) at buf[15] */

    for (idx_t i = 16; i <= n; ++i) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;

        /* compute a(i) = Σ coeff[k] * a(i‑k–1)  (k = 0..15) */
        for (int k = 0; k < 16; ++k) {
            /* index of a(i‑k‑1) in buf: (pos - k) mod 16 */
            size_t idx = (pos + 16 - k) % 16;
            if (coeff[k] >= 0) {
                sum_pos += buf[idx] * (seq_t)coeff[k];
            } else {
                sum_neg += buf[idx] * (seq_t)(-coeff[k]);
            }
        }

        seq_t next = sum_pos - sum_neg;   /* sum_pos >= sum_neg always */

        /* slide window: advance pos, overwrite oldest value */
        pos = (pos + 1) % 16;
        buf[pos] = next;
    }

    return buf[pos];
}
