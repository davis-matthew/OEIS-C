/*
 * A321971  a(n) is the number of distinct arrangements of the letters in the
 *          English word for n.
 * https://oeis.org/A321971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = factorial(total letters in English word for n) / ∏
 *            factorial(count of each letter)
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below generates the English word for numbers from 1
   up to 10 000 (no need for capitalization or "and") and counts the
   lower‑case letters.  It then returns the number of distinct
   arrangements of those letters, computed using only integer
   arithmetic.  All functions are defined with external linkage or as
   static helpers.  The implementation works for ordinary integer
   types and for arbitrary‑precision integer types (seq_t).  No
   dynamic allocation or auxiliary caching is required, so the code
   stays simple and fast. */

#include <stdbool.h>
#include <stdint.h>

/* externally defined integer types for the build system */
extern seq_t a(idx_t n);

/* English word fragments for numbers 0–19 and multiples of ten */
static const char *const units[10] = {
    "zero", "one", "two", "three", "four",
    "five", "six", "seven", "eight", "nine"
};

static const char *const teens[10] = {
    "ten", "eleven", "twelve", "thirteen", "fourteen",
    "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"
};

static const char *const tens[10] = {
    "", "", "twenty", "thirty", "forty",
    "fifty", "sixty", "seventy", "eighty", "ninety"
};

/* Add the letters of a word fragment to the count array */
static void add_word(const char *word, unsigned char cnt[26])
{
    for (; *word; ++word) {
        char c = *word;
        if (c >= 'a' && c <= 'z')
            ++cnt[c - 'a'];
    }
}

/* Recursively construct the English word for n (1 ≤ n ≤ 10 000)
   and fill the letter counter array. */
static void count_word_for_number(idx_t n, unsigned char cnt[26])
{
    if (n >= 1000) {
        idx_t thousands = n / 1000;
        idx_t rem = n % 1000;
        count_word_for_number(thousands, cnt);
        add_word("thousand", cnt);
        if (rem > 0)
            count_word_for_number(rem, cnt);
    } else if (n >= 100) {
        idx_t hundreds = n / 100;
        idx_t rem = n % 100;
        count_word_for_number(hundreds, cnt);
        add_word("hundred", cnt);
        if (rem > 0)
            count_word_for_number(rem, cnt);
    } else if (n >= 20) {
        idx_t t = n / 10;
        idx_t rem = n % 10;
        add_word(tens[t], cnt);
        if (rem > 0)
            count_word_for_number(rem, cnt);
    } else if (n >= 10) {
        add_word(teens[n - 10], cnt);
    } else {
        add_word(units[n], cnt);
    }
}

/* Main sequence function: a(n) for n ≥ 1 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* should never happen (offset starts at 1) */

    unsigned char cnt[26] = {0};
    count_word_for_number(n, cnt);

    /* total number of letters */
    idx_t total = 0;
    for (int i = 0; i < 26; ++i)
        total += cnt[i];

    /* factorial(total) */
    seq_t result = 1;
    for (idx_t i = 2; i <= total; ++i)
        result *= (seq_t)i;

    /* divide by factorial of each letter count */
    for (int i = 0; i < 26; ++i) {
        idx_t c = cnt[i];
        for (idx_t j = 2; j <= c; ++j)
            result /= (seq_t)j;
    }

    return result;
}
