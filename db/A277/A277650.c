/*
 * A277650  Numbers with primitive English number names.
 * https://oeis.org/A277650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   append
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primitive English number name sequence.
   a(n) returns the n‑th number whose English name does not contain
   the letters of any other English number name from the list:
   zero, one, two, three, four, five, six, seven, eight, nine,
   ten, eleven, twelve, thirty, forty, fifty. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/append.h"

static const char *const SMALL_WORDS[] = {
    "zero","one","two","three","four","five","six","seven","eight","nine",
    "ten","eleven","twelve","thirty","forty","fifty"
};
#define SMALL_WORD_COUNT (sizeof(SMALL_WORDS)/sizeof(SMALL_WORDS[0]))

static int SMALL_WORD_FREQS[SMALL_WORD_COUNT][26];
static bool SMALL_WORDS_INIT = false;
static void init_small_word_freqs(void)
{
    if (SMALL_WORDS_INIT) return;
    for (size_t i = 0; i < SMALL_WORD_COUNT; ++i) {
        int cnt[26] = {0};
        const char *s = SMALL_WORDS[i];
        for (const char *p = s; *p; ++p) {
            if (*p >= 'a' && *p <= 'z')
                ++cnt[*p - 'a'];
        }
        memcpy(SMALL_WORD_FREQS[i], cnt, sizeof(cnt));
    }
    SMALL_WORDS_INIT = true;
}

/* Words for constructing names */
static const char *const UNITS[] = {
    "zero","one","two","three","four","five",
    "six","seven","eight","nine","ten",
    "eleven","twelve"
};
static const char *const TENS[] = {
    "", "", "twenty","thirty","forty","fifty",
    "sixty","seventy","eighty","ninety"
};
static const char *const SCALE[] = {
    "thousand","million","billion","trillion"
};

static void build_name(uint64_t n, char *buf, size_t *len)
{
    size_t pos = 0;
    if (n < 13) {
        append(buf, &pos, UNITS[n]);
    } else if (n < 20) {
        switch ((int)n) {
            case 13: append(buf, &pos, "thirteen"); break;
            case 14: append(buf, &pos, "fourteen"); break;
            case 15: append(buf, &pos, "fifteen"); break;
            case 16: append(buf, &pos, "sixteen"); break;
            case 17: append(buf, &pos, "seventeen"); break;
            case 18: append(buf, &pos, "eighteen"); break;
            case 19: append(buf, &pos, "nineteen"); break;
        }
    } else if (n < 100) {
        int t = (int)(n / 10);
        int u = (int)(n % 10);
        append(buf, &pos, TENS[t]);
        if (u) append(buf, &pos, UNITS[u]);
    } else if (n < 1000) {
        int h = (int)(n / 100);
        int rem = (int)(n % 100);
        append(buf, &pos, UNITS[h]);
        append(buf, &pos, "hundred");
        if (rem) {
            char sub[64];
            size_t sublen;
            build_name(rem, sub, &sublen);
            append(buf, &pos, sub);
        }
    } else {
        uint64_t remaining = n;
        for (int scale_idx = 3; scale_idx >= 0; --scale_idx) {
            uint64_t scale_val = 1;
            for (int i = 0; i <= scale_idx; ++i)
                scale_val *= 1000ULL;
            uint64_t major = remaining / scale_val;
            if (major) {
                char sub[256];
                size_t sublen;
                build_name(major, sub, &sublen);
                append(buf, &pos, sub);
                append(buf, &pos, SCALE[scale_idx]);
                remaining %= scale_val;
            }
        }
        if (remaining) {
            char sub[256];
            size_t sublen;
            build_name(remaining, sub, &sublen);
            append(buf, &pos, sub);
        }
    }
    *len = pos;
}

static bool is_primitive(uint64_t n)
{
    init_small_word_freqs();
    char namebuf[512];
    size_t namelen;
    build_name(n, namebuf, &namelen);
    int freq[26] = {0};
    for (size_t i = 0; i < namelen; ++i) {
        unsigned char c = (unsigned char)namebuf[i];
        if (c >= 'a' && c <= 'z')
            ++freq[c - 'a'];
    }
    for (size_t w = 0; w < SMALL_WORD_COUNT; ++w) {
        bool subset = true, equal = true;
        for (int l = 0; l < 26; ++l) {
            if (freq[l] < SMALL_WORD_FREQS[w][l]) { subset = false; break; }
            if (freq[l] != SMALL_WORD_FREQS[w][l]) equal = false;
        }
        if (subset && !equal) return false;
    }
    return true;
}

/* ----------  Cache of primitives  ---------- */
static seq_t *CACHE = NULL;
static size_t CACHE_SIZE = 0;
static size_t CACHE_CAP = 0;
static uint64_t NEXT_CANDIDATE = 0;

static void ensure_cache(size_t need)
{
    if (need < CACHE_SIZE) return;
    if (CACHE == NULL) {
        CACHE_CAP = 64;
        CACHE = (seq_t *)malloc(CACHE_CAP * sizeof(seq_t));
        CACHE_SIZE = 0;
    }
    uint64_t cand = NEXT_CANDIDATE;
    while (CACHE_SIZE <= need) {
        if (is_primitive(cand)) {
            if (CACHE_SIZE == CACHE_CAP) {
                CACHE_CAP <<= 1;
                CACHE = (seq_t *)realloc(CACHE, CACHE_CAP * sizeof(seq_t));
            }
            CACHE[CACHE_SIZE++] = (seq_t)cand;
        }
        ++cand;
    }
    NEXT_CANDIDATE = cand;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* not defined for offset 0 */
    size_t idx = (size_t)(n - 1);
    ensure_cache(idx);
    if (idx < CACHE_SIZE) return CACHE[idx];
    return 0;                                 /* sequence finite? */
}
