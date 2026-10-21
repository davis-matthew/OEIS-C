/*
 * A277268  If we call "T" this sequence and consider the k-digit term a(n) of
 *          T with digits abcd...k, then a(n+1) = [a(n) + the a-th digit of T
 *          + the b-th digit of T + the c-th digit of T + ... + the k-th digit
 *          of T]. This is the lexicographically first such infinite sequence
 *          containing no duplicate term.
 * https://oeis.org/A277268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A277268:
   a(1) = 11
   For n ≥ 1 let the decimal digits of a(n) be a1 a2 … ak.
   Then
        a(n+1) = a(n) + Σ_{j=1..k} T_{a_j},
   where T_m is the m‑th digit of the concatenated sequence
   T = a(1) a(2) a(3) … (digits written consecutively).  */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static state used for memoisation */
    static seq_t *seq      = NULL;
    static size_t seq_cap  = 0;
    static size_t seq_len  = 0;      /* number of terms stored                       */

    static unsigned char *digits = NULL;
    static size_t digits_cap   = 0;
    static size_t digits_len   = 0;  /* number of digits stored                    */

    if (n < 1) return 0;            /* offset is 1 */

    /* initialise on first call */
    if (seq_len == 0) {
        seq_cap   = 16;
        seq       = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0]    = (seq_t)11;
        seq_len   = 1;

        digits_cap  = 64;
        digits       = (unsigned char *)malloc(digits_cap);
        digits[0]    = 1;
        digits[1]    = 1;
        digits_len   = 2;
    }

    /* grow sequence up to n */
    for (idx_t i = seq_len + 1; i <= n; ++i) {
        seq_t last   = seq[seq_len - 1];

        /* extract decimal digits of last (most‑significant first) */
        unsigned char tmp[64];
        size_t tmp_len = 0;

        if (last == 0) {
            tmp[0] = 0;
            tmp_len = 1;
        } else {
            seq_t v = last;
            while (v > 0) {
                tmp[tmp_len++] = (unsigned char)(v % (seq_t)10);
                v /= (seq_t)10;
            }
            /* reverse to get MSD → LSD */
            for (size_t l = 0, r = tmp_len - 1; l < r; ++l, --r) {
                unsigned char t = tmp[l];
                tmp[l] = tmp[r];
                tmp[r] = t;
            }
        }

        /* compute the sum of required digits of T */
        seq_t add_sum = 0;
        for (size_t j = 0; j < tmp_len; ++j) {
            unsigned char d = tmp[j];
            if (d == 0) continue;                /* digit 0 contributes nothing    */
            if ((size_t)d > digits_len) continue; /* safety – should not happen   */
            add_sum += (seq_t)digits[d - 1];
        }

        seq_t next_val = last + add_sum;

        /* append next_val to seq */
        if (seq_len == seq_cap) {
            seq_cap = seq_cap * 2;
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        }
        seq[seq_len++] = next_val;

        /* append digits of next_val to digits buffer */
        tmp_len = 0;
        if (next_val == 0) {
            tmp[0] = 0;
            tmp_len = 1;
        } else {
            seq_t v = next_val;
            while (v > 0) {
                tmp[tmp_len++] = (unsigned char)(v % (seq_t)10);
                v /= (seq_t)10;
            }
            for (size_t l = 0, r = tmp_len - 1; l < r; ++l, --r) {
                unsigned char t = tmp[l];
                tmp[l] = tmp[r];
                tmp[r] = t;
            }
        }

        size_t required = digits_len + tmp_len;
        if (required > digits_cap) {
            if (digits_cap == 0) digits_cap = 64;
            while (digits_cap < required) digits_cap <<= 1;
            digits = (unsigned char *)realloc(digits, digits_cap);
        }
        for (size_t j = 0; j < tmp_len; ++j) {
            digits[digits_len++] = tmp[j];
        }
    }

    return seq[n - 1];
}
