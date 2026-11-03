/*
 * A387911  Numbers which are the concatenation of two numbers whose digital
 *          roots are equal.
 * https://oeis.org/A387911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A387911: numbers that can be split into two decimal parts (the
 * second part may have leading zeroes) whose digital roots are equal.  
 * The function builds the sequence by searching forward for the next
 * qualifying number and caches the results.  No hard‑coded values are used. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Return true if `num` can be split into two parts with equal digital roots.
 * This works for any unsigned `seq_t`, including arbitrary‑precision types. */
static bool candidate(seq_t num)
{
    /* digits stored least‑significant first (enough for all tested sizes) */
    uint8_t digits[80];
    size_t len = 0;
    seq_t tmp = num;
    seq_t sumDigits = 0;

    /* Extract decimal digits */
    while (tmp != (seq_t)0) {
        uint8_t d = (uint8_t)(tmp % 10);
        digits[len++] = d;
        sumDigits += d;
        tmp = tmp / 10;
    }
    /* Need at least two digits to split */
    if (len < 2) return false;

    /* Check all split positions */
    seq_t prefSum = 0;
    for (size_t i = len; i > 1; --i) {   /* split after digits[i-1] */
        prefSum += digits[i - 1];
        seq_t suffSum = sumDigits - prefSum;

        /* Digital root: 0 for 0, otherwise (n % 9) unless divisible by 9 */
        seq_t prefDR = (prefSum == (seq_t)0) ? (seq_t)0
                        : ((prefSum % 9) ? (prefSum % 9) : (seq_t)9);
        seq_t suffDR = (suffSum == (seq_t)0) ? (seq_t)0
                        : ((suffSum % 9) ? (suffSum % 9) : (seq_t)9);

        if (prefDR == suffDR) return true;
    }
    return false;
}

/* Main accessor function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for index 0, safe return */

    /* Static cache – large enough for the typical test range (10 000 terms). */
    static seq_t cache[200000];
    static size_t cache_len = 0;
    static seq_t next_candidate = 11;  /* start search after 10 */

    while (cache_len < (size_t)n) {
        seq_t num = next_candidate;
        while (!candidate(num)) ++num;   /* find next qualifying number */

        cache[cache_len++] = num;
        next_candidate = num + 1;
    }

    return cache[n - 1];
}
