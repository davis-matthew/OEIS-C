/*
 * A372222  Numbers which have no imbalance when they are written in English
 *          and cut at the right place (see the definition of the imbalance in
 *          the Comments section).
 * https://oeis.org/A372222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A372222 – Numbers having a word‐balanced cut when written in English.
   The function returns the n‑th term (n ≥ 1) by searching for numbers
   that admit a tangent cut where the sum of letter values on each side
   is equal.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/*------------------------------------------------------------*/
/*  English number → letter value conversion (A=1 … Z=26)      */
/*------------------------------------------------------------*/

#define MAX_LETTERS 300

static const char *const ONES[20] = {
    "ZERO", "ONE", "TWO", "THREE", "FOUR", "FIVE", "SIX",
    "SEVEN", "EIGHT", "NINE", "TEN", "ELEVEN", "TWELVE",
    "THIRTEEN", "FOURTEEN", "FIFTEEN", "SIXTEEN", "SEVENTEEN",
    "EIGHTEEN", "NINETEEN"
};

static const char *const TENS[10] = {
    "", "", "TWENTY", "THIRTY", "FORTY", "FIFTY",
    "SIXTY", "SEVENTY", "EIGHTY", "NINETY"
};

static const char *const GROUPS[4] = {
    "TRILLION", "BILLION", "MILLION", "THOUSAND"
};
static const uint64_t MAG[4] = {1000000000000ULL,1000000000ULL,1000000ULL,1000ULL};

static inline void
push_word(const char *s, uint8_t *buf, size_t *len)
{
    for (; *s; ++s) {
        char c = *s;
        if (c >= 'A' && c <= 'Z')
            buf[(*len)++] = (uint8_t)(c - 'A' + 1);
    }
}

static void
convert_999(uint16_t x, uint8_t *buf, size_t *len)
{
    if (x >= 100) {
        uint8_t h = x / 100;
        push_word(ONES[h], buf, len);
        push_word("HUNDRED", buf, len);
        x %= 100;
    }
    if (x >= 20) {
        uint8_t t = x / 10;
        push_word(TENS[t], buf, len);
        x %= 10;
    }
    if (x > 0)
        push_word(ONES[x], buf, len);
}

static void
convert_uint64(uint64_t n, uint8_t *buf, size_t *len)
{
    if (n == 0) {
        push_word(ONES[0], buf, len);
        return;
    }
    uint64_t rem = n;
    for (int i = 0; i < 4; ++i) {
        uint64_t part = rem / MAG[i];
        rem %= MAG[i];
        if (part > 0) {
            convert_999((uint16_t)part, buf, len);
            push_word(GROUPS[i], buf, len);
        }
    }
    if (rem > 0)
        convert_999((uint16_t)rem, buf, len);
}

static bool
has_balanced_cut(uint64_t n)
{
    uint8_t letters[MAX_LETTERS];
    size_t len = 0;
    convert_uint64(n, letters, &len);

    seq_t total = 0;
    for (size_t i = 0; i < len; ++i)
        total += (seq_t)letters[i];

    seq_t left = 0;
    for (size_t i = 0; i <= len; ++i) {
        if (left == total - left)
            return true;
        if (i < len)
            left += (seq_t)letters[i];
    }
    return false;
}

/*------------------------------------------------------------*/
/*  The sequence function                                       */
/*------------------------------------------------------------*/

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t next_num = 1;

    /* grow cache until it holds n terms */
    while (cache_len < (size_t)n) {
        if (has_balanced_cut(next_num)) {
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_num;
        }
        ++next_num;
    }
    return cache[n - 1];
}
