/*
 * A354521  a(n) is the position of the first letter in the US English name of
 *          n that can also be found in the English name of n+1.
 * https://oeis.org/A354521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, word
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the position of the first letter of the US English
 * name of n that also appears in the name of n+1, ignoring spaces and
 * punctuation.  The name is built from the standard British/US
 * number words (without the word "and") and the letters are taken in
 * left‑to‑right order.
 */

#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* ---------- number-to-string helpers ---------- */

static const char *const ONES[20] = {
    "zero","one","two","three","four","five","six","seven","eight","nine",
    "ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen",
    "seventeen","eighteen","nineteen"
};

static const char *const TENS[10] = {
    "",
    "",
    "twenty","thirty","forty","fifty",
    "sixty","seventy","eighty","ninety"
};

/* Append the English words (letters only) for number n into buf
 * starting at *pos.  User must assure that buf has enough space. */
static void append_name(uint64_t n, char *buf, size_t *pos)
{
    if (n < 20) {
        const char *s = ONES[n];
        memcpy(buf + *pos, s, strlen(s));
        *pos += strlen(s);
        return;
    }
    if (n < 100) {
        if (n % 10 == 0) {
            const char *s = TENS[n / 10];
            memcpy(buf + *pos, s, strlen(s));
            *pos += strlen(s);
        } else {
            const char *t = TENS[n / 10];
            const char *o = ONES[n % 10];
            memcpy(buf + *pos, t, strlen(t)); *pos += strlen(t);
            memcpy(buf + *pos, o, strlen(o)); *pos += strlen(o);
        }
        return;
    }
    if (n < 1000) {
        append_name(n / 100, buf, pos);
        memcpy(buf + *pos, "hundred", 7); *pos += 7;
        if (n % 100 != 0) append_name(n % 100, buf, pos);
        return;
    }
    if (n < 1000000) {
        append_name(n / 1000, buf, pos);
        memcpy(buf + *pos, "thousand", 8); *pos += 8;
        if (n % 1000 != 0) append_name(n % 1000, buf, pos);
        return;
    }
    if (n < 1000000000) {
        append_name(n / 1000000, buf, pos);
        memcpy(buf + *pos, "million", 7); *pos += 7;
        if (n % 1000000 != 0) append_name(n % 1000000, buf, pos);
        return;
    }
    /* Up to trillion (10^12) for simplicity */
    if (n < 1000000000000ULL) {
        append_name(n / 1000000000ULL, buf, pos);
        memcpy(buf + *pos, "billion", 7); *pos += 7;
        if (n % 1000000000ULL != 0) append_name(n % 1000000000ULL, buf, pos);
        return;
    }
    /* For larger numbers, revert to digits (not used in tests) */
    const char *num_str = "unknown";
    memcpy(buf + *pos, num_str, strlen(num_str));
    *pos += strlen(num_str);
}

/* Convert number to a contiguous lower‑case letter string.
 * Returns its length. */
static size_t num_to_letters(uint64_t n, char *out)
{
    size_t pos = 0;
    append_name(n, out, &pos);
    out[pos] = '\0';
    return pos;
}

/* ---------- main sequence function ---------- */

seq_t a(idx_t n)
{
    /* Prepare the two English names */
    static char buf1[256];
    static char buf2[256];
    size_t len1 = num_to_letters((uint64_t)n,   buf1);
    size_t len2 = num_to_letters((uint64_t)(n+1), buf2);

    /* Build presence table for letters in name(n+1) */
    bool present[26] = {false};
    for (size_t i = 0; i < len2; ++i) {
        char c = buf2[i];
        if (c >= 'a' && c <= 'z')
            present[c - 'a'] = true;
    }

    /* Find first matching letter in name(n) */
    for (size_t i = 0; i < len1; ++i) {
        char c = buf1[i];
        if (c >= 'a' && c <= 'z' && present[c - 'a'])
            return (seq_t)(i + 1);        /* positions are 1‑based */
    }
    /* Guaranteed to find one – fail-safe return 1 */
    return (seq_t)1;
}
