/*
 * A249605  Dissectible numbers in the sense of Gunjikar and Kaprekar.
 * https://oeis.org/A249605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 25, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force search over all 3‑digit numbers (allowing leading zeros)
 * computing whether each number satisfies the dissectible property
 * with all Wonderful Demlo numbers (A002477).  The sequence a(n) contains
 * the numbers in increasing order of discovery (i.e., the numbers
 * found by the property). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

/* Wonderful Demlo numbers up to the maximum possible (m = 9),
 * written in decimal: 1, 121, 12321, ..., 12345678987654321. */
static const unsigned long long DEMLO[9] = {
    1ULL,
    121ULL,
    12321ULL,
    1234321ULL,
    123454321ULL,
    12345654321ULL,
    1234567654321ULL,
    123456787654321ULL,
    12345678987654321ULL
};

/* Helper: test a single 3‑digit integer N (1 … 999) for the dissectible property. */
static bool is_dissectible(uint64_t N)
{
    int a = (N / 100) % 10;   /* most significant digit (allowing leading 0) */
    int b = (N / 10) % 10;    /* middle digit */
    int c =  N % 10;          /* least significant digit */

    for (size_t di = 0; di < sizeof(DEMLO) / sizeof(DEMLO[0]); ++di) {
        unsigned long long prod = N * DEMLO[di];

        /* Convert product to decimal string (reversed) */
        char rev[32];
        int len = 0;
        if (prod == 0) {
            rev[len++] = '0';
        } else {
            while (prod > 0) {
                rev[len++] = (char)('0' + (prod % 10));
                prod /= 10;
            }
        }
        /* Reverse into normal order */
        for (int i = 0; i < len / 2; ++i) {
            char t = rev[i];
            rev[i] = rev[len - 1 - i];
            rev[len - 1 - i] = t;
        }

        /* Buffer that will hold the final (possibly padded) string */
        char buf[64];
        int L = len;          /* current length of product */

        if (a == 0) {    /* we may need to pad leading zeros */
            int pad = (len % 2 == 1) ? 2 : 1;   /* make length odd, add at least one zero */
            L = len + pad;
            int idx = 0;
            for (int i = 0; i < pad; ++i) buf[idx++] = '0';
            for (int i = 0; i < len; ++i) buf[idx++] = rev[i];
            buf[L] = '\0';
        } else {          /* a != 0 : no padding, but length must be odd */
            if (len % 2 == 0) return false;
            for (int i = 0; i < len; ++i) buf[i] = rev[i];
            buf[len] = '\0';
        }

        /* Solve for k and test the pattern */
        int k = (L - 3) / 2;
        if (buf[0] - '0' != a) return false;
        if (buf[L - 1] - '0' != c) return false;
        int x = buf[1] - '0';
        for (int i = 1; i <= k; ++i) {
            if (buf[i] - '0' != x) return false;
        }
        if (buf[k + 1] - '0' != b) return false;
        int y = buf[k + 2] - '0';
        for (int i = k + 2; i <= L - 2; ++i) {
            if (buf[i] - '0' != y) return false;
        }
    }
    return true;
}

/* Main sequence function requested by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    idx_t cnt = 0;
    for (idx_t N = 1; N <= 999; ++N) {
        if (is_dissectible((uint64_t)N)) {
            ++cnt;
            if (cnt == n) {
                return (seq_t)N;
            }
        }
    }
    return (seq_t)0;  /* n larger than the number of terms */
}
