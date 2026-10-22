/*
 * A382624  Positive integers such that every even digit except the leftmost
 *          is immediately preceded by a larger digit.
 * https://oeis.org/A382624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 148, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A382624: numbers where every even digit (except the
   most significant) is immediately preceded by a larger digit.
   This function finds the nth such number by brute‑force search. */

#include <stdbool.h>

static bool valid(seq_t x)
{
    /* Extract decimal digits (least significant first).  */
    unsigned char digits[200];   /* enough for any number generated here */
    int len = 0;
    seq_t tmp = x;
    do {
        digits[len++] = (unsigned char)(tmp % 10);
        tmp /= 10;
    } while (tmp > 0);

    /* For each even digit that is not the most significant digit,
       check that the preceding (more significant) digit is larger. */
    for (int i = 0; i < len - 1; ++i) {
        unsigned char d = digits[i];
        if ((d & 1) == 0) {            /* d is even */
            unsigned char prev = digits[i + 1];
            if (prev <= d) return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    seq_t cur = 1;
    idx_t cnt = 0;
    while (1) {
        if (valid(cur)) {
            ++cnt;
            if (cnt == n)
                return cur;
        }
        ++cur;
    }
}
