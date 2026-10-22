/*
 * A354523  Number of distinct letters in the English word for n that can also
 *          be found in the English word for n+1.
 * https://oeis.org/A354523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, word
 * Formula:   a(n) = number of distinct letters in the US English word for n
 *            that also appear in the word for n+1. The computation is done by
 *            generating letter bit masks for the words and intersecting them
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   mask_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mask_from_str.h"

/* Forward declarations */
static uint32_t mask_of(uint64_t n);
static uint32_t mask_from_str(const char *s);
static void init_masks(void);

/* Static data for letter masks */
static uint32_t small_masks[20];
static uint32_t tens_masks[8];
static uint32_t hundred_mask, thousand_mask, million_mask, billion_mask;
static bool masks_inited = false;

/* Initialize all the word masks once */
static void
init_masks(void)
{
    if (masks_inited)
        return;
    masks_inited = true;

    /* Small numbers 0..19 */
    static const char *small_words[20] = {
        "zero",   "one",     "two",   "three", "four",
        "five",   "six",     "seven", "eight", "nine",
        "ten",    "eleven",  "twelve","thirteen","fourteen",
        "fifteen","sixteen", "seventeen","eighteen","nineteen"
    };
    for (int i = 0; i < 20; ++i)
        small_masks[i] = mask_from_str(small_words[i]);

    /* Tens 20..90 */
    static const char *tens_words[8] = {
        "twenty",  "thirty",  "forty",  "fifty",
        "sixty",   "seventy", "eighty", "ninety"
    };
    for (int i = 0; i < 8; ++i)
        tens_masks[i] = mask_from_str(tens_words[i]);

    /* Scale words */
    hundred_mask  = mask_from_str("hundred");
    thousand_mask = mask_from_str("thousand");
    million_mask  = mask_from_str("million");
    billion_mask  = mask_from_str("billion");
}

/* Recursively compute the letter mask for n */
static uint32_t
mask_of(uint64_t n)
{
    init_masks();
    if (n == 0)
        return small_masks[0];

    /* Process billions */
    if (n >= 1000000000ULL) {
        uint64_t k = n / 1000000000ULL;
        n %= 1000000000ULL;
        uint32_t m = mask_of(k);
        m |= billion_mask;
        if (n)
            m |= mask_of(n);
        return m;
    }
    /* Process millions */
    if (n >= 1000000ULL) {
        uint64_t k = n / 1000000ULL;
        n %= 1000000ULL;
        uint32_t m = mask_of(k);
        m |= million_mask;
        if (n)
            m |= mask_of(n);
        return m;
    }
    /* Process thousands */
    if (n >= 1000ULL) {
        uint64_t k = n / 1000ULL;
        n %= 1000ULL;
        uint32_t m = mask_of(k);
        m |= thousand_mask;
        if (n)
            m |= mask_of(n);
        return m;
    }
    /* 100..999 */
    if (n >= 100) {
        uint64_t h = n / 100;
        n %= 100;
        uint32_t m = small_masks[h] | hundred_mask;
        if (n)
            m |= mask_of(n);
        return m;
    }
    /* 20..99 */
    if (n >= 20) {
        uint64_t t = (n / 10) * 10;
        uint64_t o = n % 10;
        uint32_t m = tens_masks[(t / 10) - 2];
        if (o)
            m |= small_masks[o];
        return m;
    }
    /* 0..19 */
    return small_masks[(int)n];
}

/* Main sequence function */
seq_t a(idx_t n)
{
    uint32_t m1 = mask_of((uint64_t)n);
    uint32_t m2 = mask_of((uint64_t)n + 1ULL);
    uint32_t common = m1 & m2;

    /* popcount: count set bits */
    uint32_t count = 0;
    while (common) {
        common &= common - 1;
        ++count;
    }
    return (seq_t)count;
}
