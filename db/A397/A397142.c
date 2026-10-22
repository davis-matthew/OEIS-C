/*
 * A397142  Palindromes without digit 0 whose square is also a palindrome.
 * https://oeis.org/A397142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A397142: Palindromes without digit 0 whose square is also a palindrome.
   Computed by enumerating all palindromes over digits {1,2,3} whose sum of squares
   of digits is less than 10.  For any n > 15 the sequence has no entries,
   so a(n) returns 0. */

#include <stddef.h>

static const seq_t _a397142[] = {
    1,      /* 1 */
    2,      /* 2 */
    3,      /* 3 */
    11,     /* 11 */
    22,     /* 22 */
    111,    /* 111 */
    121,    /* 121 */
    212,    /* 212 */
    1111,   /* 1111 */
    11111,  /* 11111 */
    11211,  /* 11211 */
    111111, /* 111111 */
    1111111,/* 1111111 */
    11111111,/* 11111111 */
    111111111/* 111111111 */
};

static const size_t _a397142_len = sizeof(_a397142) / sizeof(_a397142[0]);

seq_t a(idx_t n)
{
    if (n < 1 || n > _a397142_len)
        return 0;
    return _a397142[n - 1];
}
