/*
 * A366393  a(n) is the largest number that can be obtained by deleting n
 *          digits from A007908(n).
 * https://oeis.org/A366393
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 13, u32 n <= 18, u64 n <= 28, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the largest integer obtainable by deleting n
   digits from the concatenation of the integers 1..n (A007908(n)).
   The algorithm greedily keeps a lexicographically maximum subsequence
   of length |S(n)| − n. */

#include <stdlib.h>

/* count decimal digits of an idx_t value */
static idx_t digit_count(idx_t x)
{
    idx_t c = 0;
    do {
        ++c;
        x /= 10;
    } while (x);
    return c;
}

seq_t a(idx_t n)
{
    /* total number of digits in the concatenation of 1..n */
    idx_t total_len = 0;
    for (idx_t k = 1; k <= n; ++k)
        total_len += digit_count(k);

    idx_t del = n;                            /* digits to delete */
    char *stack = (char *)malloc(total_len);  /* stack for chosen digits */
    idx_t top = 0;

    /* process every integer from 1 to n */
    for (idx_t k = 1; k <= n; ++k) {
        /* temporary buffer to hold digits of k in reverse order */
        char tmpbuf[24];
        int dig_cnt = 0;
        idx_t tmp = k;
        do {
            tmpbuf[dig_cnt++] = (char)('0' + (tmp % 10));
            tmp /= 10;
        } while (tmp);

        /* output digits from most significant to least */
        for (int i = dig_cnt - 1; i >= 0; --i) {
            char d = tmpbuf[i];
            while (del && top && stack[top - 1] < d) {
                --top;
                --del;
            }
            stack[top++] = d;
        }
    }

    /* discard any remaining deletions from the end */
    if (del) {
        top -= del;
        del = 0;
    }

    /* build the final integer value */
    seq_t result = 0;
    for (idx_t i = 0; i < top; ++i)
        result = result * 10 + (seq_t)(stack[i] - '0');

    free(stack);
    return result;
}
