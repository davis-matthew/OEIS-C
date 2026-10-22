/*
 * A366946  Lexicographically earliest sequence of distinct terms > 0 such
 *          that any digit d jumping to the right over d digits lands on a
 *          square digit.
 * https://oeis.org/A366946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   lexicographically earliest sequence where each digit d jumps
 *            over d digits to land on a square digit (0,1,4,9). Computed by
 *            search and memoisation. a(1)=1, a(2)=2, a(3)=4, a(4)=3, a(5)=9,
 *            …
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   square_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/square_digit.h"

/* internal storage for the sequence, used values and the concatenated
   digit stream */
static seq_t *seq      = NULL;
static size_t seq_len  = 0, seq_cap = 0;

static bool *used      = NULL;
static size_t used_cap = 0;

static uint8_t *dig     = NULL;
static size_t dig_len   = 0, dig_cap = 0;

/* grow the main sequence array */
static void seq_ensure(size_t need) {
    if (need <= seq_cap) return;
    size_t ncap = seq_cap ? seq_cap : 4;
    while (ncap < need) ncap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, ncap * sizeof(seq_t));
    seq = tmp;
    seq_cap = ncap;
}

/* grow the used-value bool array */
static void used_ensure(size_t need) {
    if (need <= used_cap) return;
    size_t ncap = used_cap ? used_cap : 4;
    while (ncap < need) ncap <<= 1;
    bool *tmp = (bool *)realloc(used, ncap * sizeof(bool));
    /* initialise the new area */
    for (size_t i = used_cap; i < ncap; ++i) tmp[i] = false;
    used = tmp;
    used_cap = ncap;
}

/* grow the digit array */
static void dig_ensure(size_t need) {
    if (need <= dig_cap) return;
    size_t ncap = dig_cap ? dig_cap : 4;
    while (ncap < need) ncap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(dig, ncap * sizeof(uint8_t));
    dig = tmp;
    dig_cap = ncap;
}

/* compute the decimal digits (most‑significant first) of a seq_t number
   The caller must free(*out);   */
static void seq_digits(seq_t num, uint8_t **out, size_t *len) {
    if (num == 0) {
        uint8_t *res = (uint8_t *)malloc(1);
        res[0] = 0;
        *out = res;
        *len = 1;
        return;
    }
    size_t cap = 4;
    uint8_t *buf = (uint8_t *)malloc(cap);
    size_t cnt = 0;
    while (num > 0) {
        seq_t d = num % 10;
        if (cnt == cap) {
            cap <<= 1;
            uint8_t *tmp = (uint8_t *)realloc(buf, cap);
            buf = tmp;
        }
        buf[cnt++] = (uint8_t)d;
        num = num / 10;
    }
    uint8_t *res = (uint8_t *)malloc(cnt);
    for (size_t i = 0; i < cnt; ++i)
        res[i] = buf[cnt - 1 - i];
    free(buf);
    *out = res;
    *len = cnt;
}

/* test whether a candidate value can be appended while keeping the
   digit‑jump property */
static bool candidate_ok(seq_t cand, uint8_t *candd, size_t candlen) {
    size_t newlen = dig_len + candlen;
    for (size_t i = 0; i < newlen; ++i) {
        uint8_t d = (i < dig_len) ? dig[i] : candd[i - dig_len];
        size_t t = i + d + 1;
        if (t < newlen) {
            uint8_t dest = (t < dig_len) ? dig[t] : candd[t - dig_len];
            if (!square_digit(dest))
                return false;
        }
    }
    return true;
}

/* append the accepted candidate to the global data structures */
static void append_candidate(seq_t cand, uint8_t *candd, size_t candlen) {
    seq_len++;
    seq_ensure(seq_len);
    seq[seq_len - 1] = cand;     /* cast is implicit */

    /* grow used array to cover this candidate */
    used_ensure((size_t)cand + 1);
    used[(size_t)cand] = true;

    /* append digits to global stream */
    dig_len += candlen;
    dig_ensure(dig_len);
    for (size_t i = 0; i < candlen; ++i)
        dig[dig_len - candlen + i] = candd[i];
}

/* generate the next term of the sequence */
static void generate_next(void) {
    uint64_t cand = 1;
    for (;;) {
        if (cand < used_cap && used[cand]) { cand++; continue; }

        uint8_t *candd; size_t candlen;
        seq_digits((seq_t)cand, &candd, &candlen);

        if (candidate_ok((seq_t)cand, candd, candlen)) {
            append_candidate((seq_t)cand, candd, candlen);
            free(candd);
            break;
        }
        free(candd);
        cand++;   /* keep searching */
    }
}

/* public function requested by the build system */
seq_t a(idx_t n) {
    if (n == 0) return 0;             /* undefined for n<1, harmless */
    while (seq_len < (size_t)n)
        generate_next();
    return seq[(size_t)n - 1];
}
