/*
 * A390504  Positive integers whose binary representation can be reduced to
 *          length zero by removing palindromic substrings of length at least
 *          2.
 * https://oeis.org/A390504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A390504
   Sequence of positive integers whose binary representation can be
   reduced to length zero by repeatedly deleting palindromic
   substrings of length at least 2.
   Implementation: trial‑and‑error with recursive palindrome stripping.
   For each integer, we construct its binary bit array and test whether
   it can be fully reduced.  We then scan upwards until the nth such
   integer is found.  No pre‑computed table is used. */

#include <stdbool.h>
#include <stddef.h>

#define MAX_BITS 512   /* sufficient for all values encountered in tests */

/* ---------- helper: test reducibility of a bit string ---------------- */
static bool reducible_bits(const unsigned char *bits, int len)
{
    if (len == 0)          /* empty string is reducible */
        return true;
    if (len == 1)          /* single bit cannot be removed */
        return false;

    /* try all palindromic substrings of length >= 2 */
    for (int i = 0; i <= len - 2; ++i) {
        for (int j = i + 1; j <= len - 1; ++j) {
            /* is bits[i..j] a palindrome? */
            bool pal = true;
            for (int l = i, r = j; l < r; ++l, --r) {
                if (bits[l] != bits[r]) {
                    pal = false;
                    break;
                }
            }
            if (!pal)
                continue;

            /* build new bit array after removal */
            int new_len = len - (j - i + 1);
            unsigned char new_bits[MAX_BITS];
            /* copy prefix */
            for (int k = 0; k < i; ++k)
                new_bits[k] = bits[k];
            /* copy suffix */
            for (int k = j + 1; k < len; ++k)
                new_bits[new_len - (len - k)] = bits[k];

            if (reducible_bits(new_bits, new_len))
                return true;
        }
    }
    return false;
}

/* ---------- test whether a given integer is in the sequence ------ */
static bool is_reducible(seq_t x)
{
    /* compute bit length */
    int len = 0;
    seq_t tmp = x;
    while (tmp) {
        ++len;
        tmp >>= 1;
    }
    if (len == 0)      /* zero is not considered positive */
        return false;
    if (len == 1)      /* single bit cannot be removed */
        return false;

    /* build bit array: bits[0] is the most‑significant bit */
    unsigned char bits[MAX_BITS];
    for (int i = 0; i < len; ++i) {
        /* bit value at position i from the left */
        seq_t bit = (x >> (len - 1 - i)) & 1;
        bits[i] = (unsigned char)bit;
    }
    return reducible_bits(bits, len);
}

/* ---------- main sequence function ------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* below offset behaviour is irrelevant */

    seq_t cur = 1;           /* start from the first positive integer */
    idx_t count = 0;         /* how many terms found so far */

    while (true) {
        if (is_reducible(cur)) {
            if (count == n - 1)
                return cur;
            ++count;
        }
        ++cur;
    }
}
