/*
 * A380246  Elimination order of the first person in a variation of the
 *          Josephus problem, where the number of skipped people correspond to
 *          the number of letters in consecutive numbers, called
 *          SpellUnder-Down.
 * https://oeis.org/A380246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) :  the round in which person 1 is eliminated
   in the SpellUnder‑Down Josephus problem.
   The round count is simulated by stepping around the circle
   according to the number of letters in the English spelling
   of successive integers.  */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Letter counts for 0‑19 and for the tens. 0 is unused. */
static const uint8_t len01_19[20] = {
    0, /*0*/
    3, /*1 one*/
    3, /*2 two*/
    5, /*3 three*/
    4, /*4 four*/
    4, /*5 five*/
    3, /*6 six*/
    5, /*7 seven*/
    5, /*8 eight*/
    4, /*9 nine*/
    3, /*10 ten*/
    6, /*11 eleven*/
    6, /*12 twelve*/
    8, /*13 thirteen*/
    8, /*14 fourteen*/
    7, /*15 fifteen*/
    7, /*16 sixteen*/
    9, /*17 seventeen*/
    8, /*18 eighteen*/
    8  /*19 nineteen*/
};
static const uint8_t tens_len[10] = {
    0,0,6,6,5,5,5,7,6,6
}; /* 20:twenty,30:thirty,40:forty,50:fifty,
      60:sixty,70:seventy,80:eighty,90:ninety */

/* Return the number of letters in the English spelling of n
   (ignoring spaces and hyphens, no “and”).  n >= 1. */
static idx_t letterCount(idx_t n)
{
    if (n < 20)
        return (idx_t)len01_19[n];
    if (n < 100) {
        idx_t t = n / 10;
        idx_t u = n % 10;
        return (idx_t)(tens_len[t] + (u ? len01_19[u] : 0));
    }
    if (n < 1000) {
        idx_t h = n / 100;
        idx_t r = n % 100;
        idx_t cnt = len01_19[h] + 7;      /* “hundred” has 7 letters */
        if (r)
            cnt += (int)letterCount(r);
        return (idx_t)cnt;
    }
    /* For n >= 1000 and up to 10 000 (which covers all rounds
       up to n = 10 000).  Handle thousands in the same way as
       hundreds.  This covers the required range; the function
       would need more general logic for larger n, which is not
       required for this sequence. */
    idx_t thou = n / 1000;      /* 1 – 10 */
    idx_t r = n % 1000;
    idx_t cnt = len01_19[thou] + 8;   /* “thousand” has 8 letters */
    if (r)
        cnt += (int)letterCount(r);
    return (idx_t)cnt;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;   /* undefined for n = 0, but safe */

    size_t cap = (size_t)(n + 1);
    idx_t *next = (idx_t*)malloc(cap * sizeof(idx_t));
    if (!next)
        return (seq_t)0;   /* allocation failure */

    for (idx_t i = 1; i < n; ++i)
        next[i] = i + 1;
    next[n] = 1;   /* closes the circle */

    idx_t cur   = 1;
    idx_t round = 1;

    for (;;) {
        idx_t steps = letterCount(round);

        idx_t prev = cur;
        for (idx_t k = 0; k < steps; ++k) {
            prev = cur;
            cur = next[cur];
        }

        if (cur == 1) {
            free(next);
            return (seq_t)round;   /* person 1 eliminated */
        }

        /* Remove cur from the circle. */
        next[prev] = next[cur];
        cur = next[cur];
        ++round;
    }
}
