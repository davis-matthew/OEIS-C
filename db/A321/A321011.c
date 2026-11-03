/*
 * A321011  Trajectory of 86 under repeated application of the map k ->
 *          A320486(k^2).
 * https://oeis.org/A321011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 56 terms through n = 933 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=86, a(n+1)=remove_nonunique_digits(a(n)^2).  */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Remove all digits that occur more than once in the decimal representation
   of k.  For example 54700816 --> 547816.  */
static seq_t remove_nonunique_digits(seq_t k)
{
    if (k == (seq_t)0) return (seq_t)0;

    /* 100 digits is more than enough for the numbers that appear in this
       10‑term cycle.  */
    uint8_t digits[100];
    int len = 0;
    while (k > (seq_t)0) {
        digits[len++] = (uint8_t)(k % (seq_t)10);
        k /= (seq_t)10;
    }

    unsigned char freq[10] = {0};
    for (int i = 0; i < len; ++i) {
        freq[digits[i]]++;
    }

    seq_t result = (seq_t)0;
    /* Build number in original left‑to‑right order. */
    for (int i = len - 1; i >= 0; --i) {
        uint8_t d = digits[i];
        if (freq[d] == 1) {
            result = result * (seq_t)10 + d;
        }
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0; /* undefined for offset 0 */

    static seq_t cycle[10];
    static bool initialized = false;

    if (!initialized) {
        cycle[0] = (seq_t)86;
        for (int i = 1; i < 10; ++i) {
            seq_t sq = seq_mul(cycle[i - 1], cycle[i - 1]);   /* k^2 */
            cycle[i] = remove_nonunique_digits(sq);
        }
        initialized = true;
    }

    idx_t pos = (n - (idx_t)1) % (idx_t)10;
    return cycle[(int)pos];
}
