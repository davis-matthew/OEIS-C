/*
 * A278217  Filter-sequence related to base-2 run-length encoding: a(n) =
 *          A046523(A075159(1+n)) = A046523(1+A075157(n)).
 * https://oeis.org/A278217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A046523(1+A075157(n)); the exponents of 1+A075157(n) are
 *            the binary run lengths of n (minus 1 each, except the top run),
 *            sorted descending and assigned to the primes 2,3,5,
 * Range:     u8 n <= 134, u16 n <= 2246, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 114 terms through n = 9909 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static const int primes[64] = {
        2,   3,   5,   7,  11,  13,  17,  19,  23,  29,
       31,  37,  41,  43,  47,  53,  59,  61,  67,  71,
       73,  79,  83,  89,  97, 101, 103, 107, 109, 113,
      127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
      179, 181, 191, 193, 197, 199, 211, 223, 227, 229,
      233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
      283, 293, 307, 311
    };
    int ex[64];
    int k = 0;
    idx_t t = n;
    while (t != 0 && k < 64) {
        int bit = (int)(t & 1);
        int len = 0;
        while (t != 0 && (int)(t & 1) == bit) {
            len++;
            t >>= 1;
        }
        /* top run (t == 0 now) is of 1s and keeps its full length */
        ex[k++] = (t == 0) ? len : len - 1;
    }
    /* insertion sort, descending */
    for (int i = 1; i < k; i++) {
        int v = ex[i];
        int j = i - 1;
        while (j >= 0 && ex[j] < v) {
            ex[j + 1] = ex[j];
            j--;
        }
        ex[j + 1] = v;
    }
    seq_t r = 1;
    for (int i = 0; i < k; i++) {
        for (int e = 0; e < ex[i]; e++)
            r *= primes[i];
    }
    return r;
}
