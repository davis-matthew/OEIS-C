/*
 * A257043  Integers with the same first and last letter in their English
 *          name.
 * https://oeis.org/A257043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, word
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 3900, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   write_word
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumerates integers whose English name starts and ends with the same letter (sequence A257043) */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/write_word.h"

/* English word parts */
static const char *UNITS[] = {
    "zero","one","two","three","four","five","six","seven","eight","nine",
    "ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen",
    "seventeen","eighteen","nineteen"
};

static const char *TENS[] = {
    "", "", "twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety"
};

/* Recursive routine to build the English name of n (0 < n) */
static void write_num_rec(unsigned long long n, char *buf, size_t *pos, bool *first)
{
    if (n >= 1000000000ULL) {
        write_num_rec(n / 1000000000ULL, buf, pos, first);
        write_word(buf, pos, "billion", first);
        n %= 1000000000ULL;
    }
    if (n >= 1000000ULL) {
        write_num_rec(n / 1000000ULL, buf, pos, first);
        write_word(buf, pos, "million", first);
        n %= 1000000ULL;
    }
    if (n >= 1000ULL) {
        write_num_rec(n / 1000ULL, buf, pos, first);
        write_word(buf, pos, "thousand", first);
        n %= 1000ULL;
    }
    if (n >= 100ULL) {
        write_num_rec(n / 100ULL, buf, pos, first);
        write_word(buf, pos, "hundred", first);
        n %= 100ULL;
    }
    if (n >= 20ULL) {
        write_word(buf, pos, TENS[n / 10], first);
        n %= 10ULL;
    }
    if (n > 0) {
        write_word(buf, pos, UNITS[n], first);
    }
}

/* Return true if the English name of num starts and ends with the same letter */
static bool same_first_last_letter(unsigned long long num)
{
    char buf[256];
    size_t pos = 0;
    bool first_flag = true;
    write_num_rec(num, buf, &pos, &first_flag);
    if (pos == 0) return false;
    buf[pos] = '\0';

    unsigned char first = 0, last = 0;
    for (size_t i = 0; i < pos; ++i) {
        if (buf[i] > ' ') {
            first = buf[i];
            break;
        }
    }
    for (size_t i = pos; i > 0; --i) {
        if (buf[i - 1] > ' ') {
            last = buf[i - 1];
            break;
        }
    }
    return first == last && first != 0;
}

/* Memoization cache for arbitrary order of calls */
static seq_t *CACHE = NULL;
static size_t CACHE_CAP = 0;
static idx_t CACHE_LEN = 0;
static unsigned long long NEXT_CAND = 1;

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 1, but return 0 safely */

    while (CACHE_LEN < n) {
        unsigned long long cand = NEXT_CAND++;
        if (same_first_last_letter(cand)) {
            if (CACHE_LEN == CACHE_CAP) {
                size_t new_cap = CACHE_CAP ? CACHE_CAP * 2 : 128;
                seq_t *tmp = (seq_t *)realloc(CACHE, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                CACHE = tmp;
                CACHE_CAP = new_cap;
            }
            CACHE[CACHE_LEN++] = (seq_t)cand;
        }
    }
    return CACHE[n - 1];
}
