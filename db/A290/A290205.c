/*
 * A290205  Group identifier of iterated Scrabble function.
 * https://oeis.org/A290205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) is obtained by repeatedly replacing n with the Scrabble
 *            score of its English word until the value becomes 12 (fixed
 *            point) or enters the 4‑7‑8‑9 cycle. The result is 12 or 4
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* letter values in Scrabble (A=1, B=3, C=3, D=2, E=1, F=4, G=2, H=4,
   I=1, J=8, K=5, L=1, M=3, N=1, O=1, P=3, Q=10, R=1, S=1, T=1,
   U=1, V=4, W=4, X=8, Y=4, Z=10) */
static const int letter_score[26] = {
    1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3,10, 1, 1, 1, 1, 4, 4, 8, 4,10
};

/* basic word lists */
static const char *units[] = {"ZERO","ONE","TWO","THREE","FOUR","FIVE","SIX","SEVEN","EIGHT","NINE"};
static const char *teens[] = {"TEN","ELEVEN","TWELVE","THIRTEEN","FOURTEEN",
                              "FIFTEEN","SIXTEEN","SEVENTEEN","EIGHTEEN","NINETEEN"};
static const char *tens[]  = {"","","TWENTY","THIRTY","FORTY","FIFTY",
                              "SIXTY","SEVENTY","EIGHTY","NINETY"};
static const char *scales[] = {"THOUSAND","MILLION","BILLION","TRILLION",
                               "QUADRILLION","QUINTILLION"};   /* up to 10^18 */

/* calculate Scrabble score of a word (letters only) */
static seq_t word_score(const char *w) {
    seq_t s = 0;
    while (*w) {
        char c = *w++;
        if (c >= 'A' && c <= 'Z')
            s += letter_score[c - 'A'];
        else if (c >= 'a' && c <= 'z')
            s += letter_score[c - 'a'];
    }
    return s;
}

/* score for numbers 1..999 */
static seq_t score_1_to_999(unsigned int n) {
    seq_t total = 0;
    if (n >= 100) {
        unsigned int h = n / 100;
        total += word_score(units[h]);
        total += word_score("HUNDRED");
        n %= 100;
    }
    if (n >= 20) {
        unsigned int t = n / 10;
        total += word_score(tens[t]);
        n %= 10;
    }
    if (n > 0) {
        if (n < 10)
            total += word_score(units[n]);
        else
            total += word_score(teens[n - 10]);
    }
    return total;
}

/* score for any unsigned 64‑bit number */
static seq_t scrabble_score_uint64(uint64_t n) {
    if (n == 0) return word_score("ZERO");

    seq_t total = 0;
    int scale_idx = 0;            /* 0 = none, 1 = thousand, 2 = million, ... */
    while (n > 0) {
        unsigned int group = n % 1000;
        n /= 1000;
        if (group != 0) {
            total += score_1_to_999(group);
            if (scale_idx > 0)
                total += word_score(scales[scale_idx - 1]);  /* scales[0] = THOUSAND */
        }
        ++scale_idx;
    }
    return total;
}

/* wrapper that accepts seq_t (big integer) but we only need small values
   so we cast to uint64_t safely           */
static seq_t scrabble_score(seq_t x) {
    return scrabble_score_uint64((uint64_t)x);
}

/* main sequence function */
extern seq_t a(idx_t n) {
    seq_t s = scrabble_score((uint64_t)n);
    while (s != 12 && s != 4) {
        s = scrabble_score(s);
    }
    return s;   /* s is 12 or 4 already */
}
