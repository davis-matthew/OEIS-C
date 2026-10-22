/*
 * A361746  Number of occurrences of the most frequently occurring letter(s)
 *          in US English name of n.
 * https://oeis.org/A361746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, word, less
 * Formula:   a(n) = maximum frequency of a letter in the US English name of n
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   add_counts_of_word
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/add_counts_of_word.h"

/* mapping for numbers 0–19 */
static const char * const units[] = {
    "zero", "one", "two", "three", "four", "five",
    "six", "seven", "eight", "nine", "ten",
    "eleven", "twelve", "thirteen", "fourteen",
    "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"
};

/* mapping for tens 20–90 */
static const char * const tens[] = {
    "twenty", "thirty", "forty", "fifty",
    "sixty", "seventy", "eighty", "ninety"
};

/* group names for thousands */
static const char * const group_names[] = {
    "", "thousand", "million", "billion",
    "trillion", "quadrillion", "quintillion"
};

/* process a 1–999 chunk and the optional group name */
static void process_chunk(uint64_t chunk, const char *group_name, int counts[26]) {
    if (chunk >= 100) {
        int h = chunk / 100;                 /* 1–9 */
        add_counts_of_word(units[h], counts);
        add_counts_of_word("hundred", counts);
        chunk %= 100;
    }
    if (chunk >= 20) {
        int t = chunk / 10;                  /* 2–9 */
        add_counts_of_word(tens[t - 2], counts);
        chunk %= 10;
    }
    if (chunk > 0) {                          /* 1–19 */
        add_counts_of_word(units[chunk], counts);
    }
    if (group_name[0]) {
        add_counts_of_word(group_name, counts);
    }
}

/* main sequence function */
seq_t a(idx_t n) {
    /* a(0) = "zero" → every letter appears once */
    if (n == 0)
        return 1;

    int counts[26] = {0};
    uint64_t temp = n;
    size_t group_index = 0;

    while (temp > 0) {
        uint64_t chunk = temp % 1000;          /* 0–999 */
        if (chunk > 0) {
            process_chunk(chunk, group_names[group_index], counts);
        }
        temp /= 1000;
        if (group_index + 1 < sizeof(group_names) / sizeof(group_names[0]))
            ++group_index;
    }

    /* find the maximum frequency */
    seq_t max = 0;
    for (int i = 0; i < 26; ++i) {
        seq_t f = (seq_t)counts[i];
        if (f > max)
            max = f;
    }
    return max;
}
