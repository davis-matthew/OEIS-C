/*
 * A381129  A version of the Josephus problem: a(n) is the surviving integer
 *          under the spelling version of the elimination process, called
 *          Down-SpellUnder.
 * https://oeis.org/A381129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A381129: Down-SpellUnder Josephus variant
   Algorithm: simulate elimination. After eliminating 1 we
   repeatedly skip a number of people equal to the number
   of letters in the English spelling of the current
   elimination count (1 → "one", 2 → "two", 3 → "three", …).
   The person landed on after the spelling is eliminated.
   New current is the next person clockwise after the eliminated
   one.  The process continues until one person remains.
*/
#include <stdlib.h>
#include <stdint.h>

static int letter_count(uint64_t num)
{
    /* counts letters of English spelling (US, no spaces or hyphens) */
    static const int unit_len[10] = {0,3,3,5,4,4,3,5,5,4};
    static const int teen_len[10] = {3,6,6,8,8,7,7,9,8,8};
    static const int tens_len[10] = {0,0,6,6,5,5,5,7,6,6};

    int len = 0;
    if (num >= 1000) {
        int thousands = (int)(num / 1000);       /* 1 .. 9 */
        len += unit_len[thousands] + 8;          /* "thousand" */
        num %= 1000;
        if (num == 0) return len;
    }
    if (num >= 100) {
        int hundreds = (int)(num / 100);         /* 1 .. 9 */
        len += unit_len[hundreds] + 7;           /* "hundred" */
        num %= 100;
        if (num == 0) return len;
    }
    if (num >= 20) {
        int tens = (int)(num / 10);              /* 2 .. 9 */
        len += tens_len[tens];
        num %= 10;
        if (num == 0) return len;
    }
    if (num >= 10) {
        len += teen_len[num - 10];
    } else if (num > 0) {
        len += unit_len[num];
    }
    return len;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n == 1) return 1;

    /* circular linked list via next pointers */
    idx_t *next = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (!next) return 0;          /* allocation failure fallback */

    for (idx_t i = 1; i < n; ++i) next[i] = i + 1;
    next[n] = 1;                   /* complete circle */

    /* remove person 1: link n to 2 */
    next[n] = next[1];             /* now next[n] points to 2 */
    idx_t cur = next[1];           /* current start for further eliminations */
    idx_t size = n - 1;            /* remaining people */

    /* precompute letter counts for all required words */
    int *len_arr = (int *)malloc((n + 1) * sizeof(int));
    if (!len_arr) { free(next); return 0; }
    for (idx_t i = 1; i <= n; ++i) len_arr[i] = letter_count(i);

    idx_t k = 1;   /* word number corresponding to the next elimination */
    while (size > 1) {
        int len = len_arr[k];
        idx_t s = len % size;
        if (s == 0) s = size;           /* full circle brings back to cur */

        /* find the node before the one to eliminate */
        idx_t prev = cur;
        for (idx_t i = 1; i < s; ++i) prev = next[prev];

        idx_t elim = next[prev];        /* node to be eliminated */

        /* remove elim from the circle */
        next[prev] = next[elim];
        cur = next[elim];              /* next current is after elim */

        size--;                         /* one less person now */
        k++;                            /* next word */
    }

    free(next);
    free(len_arr);
    return (seq_t)cur;
}
