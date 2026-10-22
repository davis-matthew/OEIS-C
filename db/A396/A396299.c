/*
 * A396299  RUNS transform of A005606 (initial letter of English name for n).
 * https://oeis.org/A396299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the n-th run of consecutive identical initial
 *            letters in the English names of the non‑negative integers. The
 *            sequence begins: 1,1,2,2,2,1,1,1,1,2,2,2,1,1,20,20,20,10,10,100,
 *            200,200,200,100,
 * Range:     u8 n <= 24, u16 n <= 42, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const char digit_letters[10]   = {'Z','O','T','T','F','F','S','S','E','N'};
static const char teen_letters[10]    = {'T','E','T','T','F','F','S','S','E','N'};
static const char tens_letters[10]    = {'\0','\0','T','T','F','F','S','S','E','N'};

static char first_letter_small(idx_t n)
/* Return the first letter (capital) of the English name for 1 ≤ n ≤ 999.
   0 is never used except in special case for the digit 0 itself. */
{
    if (n < 10) return digit_letters[n];
    if (n < 20) return teen_letters[n-10];
    if (n < 100) return tens_letters[n/10];
    /* 100 ≤ n < 1000 → first word is the hundreds digit */
    return digit_letters[(n/100)];
}

static char first_letter_big(idx_t n)
/* Return the first letter of the English name for any non‑negative integer.
   For n > 0 the most significant 3‑digit group determines the first word. */
{
    if (n == 0) return 'Z';

    idx_t h = n;
    while (h >= 1000) h /= 1000;          /* isolate the leading 3‐digit block */
    return first_letter_small(h);
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;                 /* first run Z, length 1 */

    idx_t run_index = 0;
    char cur = first_letter_big(0);       /* 'Z' */
    seq_t len = 1;
    idx_t i;

    for (i = 1; ; ++i) {
        char nxt = first_letter_big(i);
        if (nxt != cur) {                 /* run ends here */
            if (run_index == n) return len;
            ++run_index;
            cur = nxt;
            len = 1;
        } else {
            ++len;
        }
    }
}
