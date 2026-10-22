/*
 * A303701  Number of distinct letters in the (American) English name of n,
 *          excluding spaces and hyphens.
 * https://oeis.org/A303701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: number of distinct letters in the American English name
   of n (excluding spaces and hyphens) */

#include <stdint.h>

/* Add the letters of a literal word to the mask */
static inline void add_word(const char *w, uint32_t *mask)
{
    for (const char *p = w; *p; ++p)
        *mask |= 1u << (*p - 'a');
}

/* Count set bits in a 32‑bit mask */
static inline int popcount32(uint32_t m)
{
    int c = 0;
    while (m) { m &= m - 1; ++c; }
    return c;
}

/* Word tables for numbers */
static const char *const units_words[10] =
    { "zero", "one", "two", "three", "four", "five", "six", "seven",
      "eight", "nine" };
static const char *const teens_words[10] =
    { "ten", "eleven", "twelve", "thirteen", "fourteen",
      "fifteen", "sixteen", "seventeen", "eighteen", "nineteen" };
static const char *const tens_words[10] =
    { "", "", "twenty", "thirty", "forty", "fifty", "sixty",
      "seventy", "eighty", "ninety" };

static const char *const hundred_word  = "hundred";
static const char *const thousand_word = "thousand";

/* Process a 0‑999 chunk and OR its letters into mask */
static inline void process_chunk(uint64_t chunk, uint32_t *mask)
{
    if (chunk < 20) {
        if (chunk < 10)
            add_word(units_words[chunk], mask);
        else
            add_word(teens_words[chunk - 10], mask);
    } else if (chunk < 100) {
        int tens = (int)(chunk / 10);
        add_word(tens_words[tens], mask);
        int ones = (int)(chunk % 10);
        if (ones) add_word(units_words[ones], mask);
    } else { /* 100‑999 */
        int hundreds = (int)(chunk / 100);
        add_word(units_words[hundreds], mask);
        add_word(hundred_word, mask);
        uint64_t rem = chunk % 100;
        if (rem) process_chunk(rem, mask);
    }
}

/* Size of the array below covers numbers up to quadrillion
   (enough for all uint64_t values). */
static const char *const magnitude_words[] =
    { "", "thousand", "million", "billion", "trillion",
      "quadrillion", "quintillion" };

/* Main function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)4;   /* "zero" */

    uint32_t mask = 0;
    idx_t value = n;
    int group = 0;  /* 0 – units, 1 – thousand, 2 – million, ... */

    while (value > 0) {
        uint64_t chunk = value % 1000;
        if (chunk != 0) {
            process_chunk(chunk, &mask);
            if (group > 0)
                add_word(magnitude_words[group], &mask);
        }
        value /= 1000;
        ++group;
    }

    return (seq_t)popcount32(mask);
}
