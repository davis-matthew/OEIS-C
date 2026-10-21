/*
 * A269592  Digits of one of the two 5-adic integers sqrt(-4). Here the ones
 *          related to A269590.
 * https://oeis.org/A269592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A269592 – Digits of one of the two 5‑adic integers sqrt(-4)
 *
 *  Recurrence used:
 *    a(0) = 4
 *    For n ≥ 1 let
 *      e = ((∑_{k=0}^{n-1} a(k)*5^k)^2 + 4) / 5^n  (mod 5)
 *      a(n) = −e·(2·a(0))^(-1)  (mod 5)   (*)
 *    Since a(0)=4, 2·a(0)=8≡3 (mod 5) and the inverse of 3 mod 5 is 2.
 *
 *  The implementation keeps the convolution S[n] = (∑_{k=0}^{n-1} a(k)*5^k)^2
 *  in a natural  base‑5 representation, so that e is obtained by
 *  dividing S + 4 by 5^n.  All operations are carried out in ordinary
 *  C/C++ integer types; no arbitrary‑precision arithmetic is needed
 *  because all intermediate values stay below 2^32 for n ≤ 10 000.
 *
 *  The function is memoised; calls may come in any order.
 *  It compiles in C11 and C++17 with the supplied `seq_t` and `idx_t` types.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* static state ----------------------------------------------------------- */
static unsigned char *digits = NULL;   /* computed a(n) values (0 … 4)    */
static size_t         digits_len = 0;  /* number of digits currently known */
static unsigned int  *sq      = NULL;   /* convolution sums in base‑5      */
static size_t         sq_len   = 0;      /* allocated size of sq array      */

/* enlarge sq to at least new_len entries, initialising new cells to 0 */
static void enlarge_sq(size_t new_len)
{
    if (sq_len < new_len) {
        unsigned int *tmp = (unsigned int *)realloc(sq, new_len * sizeof(unsigned int));
        if (!tmp) abort();                     /* allocation failure */
        for (size_t i = sq_len; i < new_len; ++i) tmp[i] = 0;
        sq = tmp;
        sq_len = new_len;
    }
}

/* enlarge digits storage to at least new_len entries (no initialisation) */
static void enlarge_digits(size_t new_len)
{
    if (digits_len < new_len) {
        unsigned char *tmp = (unsigned char *)realloc(digits, new_len * sizeof(unsigned char));
        if (!tmp) abort();                     /* allocation failure */
        digits = tmp;
    }
}

/* public function -------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* initialise on first call */
    if (digits_len == 0) {
        enlarge_digits(1);
        digits[0] = 4u;                      /* a(0) = 4 */
        enlarge_sq(1);
        sq[0]   = 4u * 4u;                    /* (a0)^2 = 16 */
        digits_len = 1;
    }

    /* extend the table until a(n) is known */
    while (digits_len <= (size_t)n) {
        size_t k = digits_len;              /* index of next digit to compute */

        enlarge_sq(k + 1);                  /* we will need sq[0..k] */

        /* compute e = ((sum a[i]*5^i)^2 + 4) / 5^k (mod 5) */
        unsigned int carry = 0, e = 0;
        for (size_t i = 0; i <= k; ++i) {
            unsigned int val = sq[i];
            if (i == 0) val += 4u;          /* add the constant 4 */
            val += carry;
            unsigned int digit = val % 5u;
            carry = val / 5u;
            if (i == k) e = digit;          /* digit at position k */
        }

        /* a(k) = -(e * 2) mod 5  (since inv(3)=2 mod 5) */
        unsigned int a_k = (5u - ((e * 2u) % 5u)) % 5u;  /* 0…4 */

        /* append new digit */
        enlarge_digits(digits_len + 1);
        digits[digits_len] = (unsigned char)a_k;
        digits_len++;

        /* update convolution array: add contributions of a_k */
        for (size_t i = 0; i < k; ++i) {
            size_t m = i + k;
            enlarge_sq(m + 1);
            sq[m] += 2u * digits[i] * a_k;
        }
        size_t m = 2u * k;
        enlarge_sq(m + 1);
        sq[m] += a_k * a_k;
    }

    return (seq_t)digits[(size_t)n];
}
