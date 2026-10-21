/*
 * A287277  Number of set partitions of [n] such that for each block all
 *          absolute differences between consecutive elements are <= five.
 * https://oeis.org/A287277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A287214(n,5) Recurrence: a(n) = 6*a(n‑1) – 6*a(n‑2) –
 *            4*a(n‑3) – 3*a(n‑4) – 28*a(n‑5) + 54*a(n‑6) + 16*a(n‑7) –
 *            16*a(n‑8) + 6*a(n‑9) – 30*a(n‑10) – 4*a(n‑11) + 13*a(n‑12) +
 *            2*a(n‑14) – a(n‑16) Initial values (a(0) … a(15)): 1, 1, 2, 5,
 *            15, 52, 203, 825, 3442, 14589, 62361, 267663, 1151563,...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t init[16] = {
    (seq_t)1,    /* a(0) */
    (seq_t)1,    /* a(1) */
    (seq_t)2,    /* a(2) */
    (seq_t)5,    /* a(3) */
    (seq_t)15,   /* a(4) */
    (seq_t)52,   /* a(5) */
    (seq_t)203,  /* a(6) */
    (seq_t)825,  /* a(7) */
    (seq_t)3442, /* a(8) */
    (seq_t)14589,/* a(9) */
    (seq_t)62361,/* a(10) */
    (seq_t)267663,/* a(11) */
    (seq_t)1151563,/* a(12) */
    (seq_t)4960725,/* a(13) */
    (seq_t)21384434,/* a(14) */
    (seq_t)92216247 /* a(15) */
};

static const int coeff[16] = {
    6,  -6,  -4,  -3,  -28, 54, 16, -16, 6, -30,
   -4, 13, 0, 2, 0, -1
};

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;
    static idx_t buf_len = 0;
    static idx_t computed_len = 0;  /* highest index computed so far */

    if (n < 16) {
        return init[n];
    }

    if (buf == NULL) {
        buf_len = 16;
        buf = (seq_t *)malloc(buf_len * sizeof(seq_t));
        if (!buf) return (seq_t)0;
        for (idx_t i = 0; i < 16; ++i) buf[i] = init[i];
        computed_len = 15;
    }

    if (n <= computed_len) {
        return buf[n];
    }

    if (n >= buf_len) {
        idx_t new_len = buf_len;
        while (new_len <= n) new_len <<= 1;  /* grow exponentially */
        seq_t *tmp = (seq_t *)realloc(buf, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        buf = tmp;
        buf_len = new_len;
    }

    for (idx_t idx = computed_len + 1; idx <= n; ++idx) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        for (int j = 0; j < 16; ++j) {
            int c = coeff[j];
            if (c == 0) continue;
            seq_t term = buf[idx - j - 1];
            if (c > 0) {
                sum_pos += term * (seq_t)c;
            } else {
                sum_neg += term * (seq_t)(-c);
            }
        }
        buf[idx] = sum_pos - sum_neg;
    }

    computed_len = n;
    return buf[n];
}
