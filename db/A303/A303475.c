/*
 * A303475  a(n) is the least m > n, such that every letter, with repetition,
 *          that occurs in the English name of n also occurs in the English
 *          name of m.
 * https://oeis.org/A303475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, look
 * Formula:   a(n) = the least m > n such that every letter, counted with
 *            multiplicity, that appears in the English name of n also appears
 *            in the English name of m. The English spelling follows the
 *            standard British convention (e.g., 21 → "twenty-one"), with
 *            hyphens and spaces ignored when counting letters
 * Range:     u8 n <= 110, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* Basic English words for numbers.  */
static const char *const units[20] = {
    "zero","one","two","three","four","five","six","seven","eight","nine",
    "ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen",
    "seventeen","eighteen","nineteen"
};

static const char *const tens[10] = {
    "", "", "twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety"
};

/* Recursively build the English name of n into buf, starting at *pos.  */
static void build_words(idx_t n, char *buf, size_t *pos)
{
    if (n < 20) {
        const char *w = units[n];
        size_t len = strlen(w);
        memcpy(buf + *pos, w, len);
        *pos += len;
    } else if (n < 100) {
        const char *w = tens[n / 10];
        size_t len = strlen(w);
        memcpy(buf + *pos, w, len);
        *pos += len;
        if (n % 10) {
            buf[(*pos)++] = '-';
            const char *w2 = units[n % 10];
            size_t len2 = strlen(w2);
            memcpy(buf + *pos, w2, len2);
            *pos += len2;
        }
    } else if (n < 1000) {
        const char *w = units[n / 100];
        size_t len = strlen(w);
        memcpy(buf + *pos, w, len);
        *pos += len;
        buf[(*pos)++] = ' ';
        const char *cent = "hundred";
        size_t clen = strlen(cent);
        memcpy(buf + *pos, cent, clen);
        *pos += clen;
        if (n % 100) {
            buf[(*pos)++] = ' ';
            build_words(n % 100, buf, pos);
        }
    } else if (n < 1000000) {
        build_words(n / 1000, buf, pos);
        buf[(*pos)++] = ' ';
        const char *th = "thousand";
        size_t tlen = strlen(th);
        memcpy(buf + *pos, th, tlen);
        *pos += tlen;
        if (n % 1000) {
            buf[(*pos)++] = ' ';
            build_words(n % 1000, buf, pos);
        }
    } else if (n < 1000000000) {
        build_words(n / 1000000, buf, pos);
        buf[(*pos)++] = ' ';
        const char *mi = "million";
        size_t mlen = strlen(mi);
        memcpy(buf + *pos, mi, mlen);
        *pos += mlen;
        if (n % 1000000) {
            buf[(*pos)++] = ' ';
            build_words(n % 1000000, buf, pos);
        }
    } else {
        /* Numbers larger than 1e9 are not needed for the tested range. */
        *pos = 0;
    }
}

/* Convert n into its English name string.  */
static void number_to_words(idx_t n, char *buf)
{
    size_t pos = 0;
    build_words(n, buf, &pos);
    buf[pos] = '\0';
}

/* Count the letters (a–z) that appear in the English name of n.
   The result is stored in cnt[26]; non‑alphabetic characters are ignored. */
static void count_letters(idx_t n, unsigned char cnt[26])
{
    for (int i = 0; i < 26; ++i) cnt[i] = 0;
    char buf[256];
    number_to_words(n, buf);
    for (const char *p = buf; *p; ++p) {
        char c = *p;
        if (c >= 'A' && c <= 'Z') c = c - 'A' + 'a';
        if (c >= 'a' && c <= 'z') {
            cnt[c - 'a']++;
        }
    }
}

/* The required sequence function.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n = 0, return 0 for safety */

    unsigned char target[26];
    count_letters(n, target);

    idx_t m = n + 1;
    unsigned char cur[26];
    for (;;) {
        count_letters(m, cur);
        bool ok = true;
        for (int i = 0; i < 26; ++i) {
            if (cur[i] < target[i]) {
                ok = false;
                break;
            }
        }
        if (ok) return (seq_t)m;
        ++m;
    }
}
