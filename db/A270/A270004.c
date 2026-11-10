/*
 * A270004  Run-length encoding of iterated Scrabble function.
 * https://oeis.org/A270004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = length of the n-th run (n = 0, 1, ...) of equal
 *            consecutive terms in A290205, where A290205(m) = 12 or 4 is the
 *            end of the iterated Scrabble-score map (computed by direct scan)
 * Range:     u8 n <= 455, u16 n <= 455, u32 n <= 455, u64 n <= 455,
 *            u128 n <= 455, big any n
 * Validated: standard, pass, 121 terms through n = 452 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static const int A270004_ls[26] = {
    1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3,10, 1, 1, 1, 1, 4, 4, 8, 4,10
};
static const char *A270004_units[] = {"ZERO","ONE","TWO","THREE","FOUR","FIVE","SIX","SEVEN","EIGHT","NINE"};
static const char *A270004_teens[] = {"TEN","ELEVEN","TWELVE","THIRTEEN","FOURTEEN",
                                      "FIFTEEN","SIXTEEN","SEVENTEEN","EIGHTEEN","NINETEEN"};
static const char *A270004_tens[]  = {"","","TWENTY","THIRTY","FORTY","FIFTY",
                                      "SIXTY","SEVENTY","EIGHTY","NINETY"};
static const char *A270004_scales[] = {"THOUSAND","MILLION","BILLION","TRILLION",
                                       "QUADRILLION","QUINTILLION"};

static uint64_t A270004_ws(const char *w) {
    uint64_t s = 0;
    while (*w) {
        char c = *w++;
        if (c >= 'A' && c <= 'Z') s += A270004_ls[c - 'A'];
    }
    return s;
}

static uint64_t A270004_s999(unsigned int n) {
    uint64_t t = 0;
    if (n >= 100) {
        t += A270004_ws(A270004_units[n / 100]);
        t += A270004_ws("HUNDRED");
        n %= 100;
    }
    if (n >= 20) {
        t += A270004_ws(A270004_tens[n / 10]);
        n %= 10;
    }
    if (n > 0) {
        if (n < 10) t += A270004_ws(A270004_units[n]);
        else t += A270004_ws(A270004_teens[n - 10]);
    }
    return t;
}

static uint64_t A270004_score(uint64_t n) {
    if (n == 0) return A270004_ws("ZERO");
    uint64_t total = 0;
    int idx = 0;
    while (n > 0) {
        unsigned int g = (unsigned int)(n % 1000);
        n /= 1000;
        if (g != 0) {
            total += A270004_s999(g);
            if (idx > 0) total += A270004_ws(A270004_scales[idx - 1]);
        }
        ++idx;
    }
    return total;
}

static uint64_t A270004_group(uint64_t m) {
    uint64_t s = A270004_score(m);
    while (s != 12 && s != 4) s = A270004_score(s);
    return s;
}

extern seq_t a(idx_t n) {
    uint64_t cur = A270004_group(0);
    uint64_t run = 0, len = 1;
    for (uint64_t m = 1;; ++m) {
        uint64_t v = A270004_group(m);
        if (v == cur) {
            ++len;
        } else {
            if (run == (uint64_t)n) return (seq_t)len;
            ++run;
            cur = v;
            len = 1;
        }
    }
}
