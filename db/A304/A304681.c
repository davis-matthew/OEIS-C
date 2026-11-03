/*
 * A304681  Smallest odd positive integer t such that the prefix of length tn
 *          of the Thue-Morse sequence (A010060) is an n-antipower.
 * https://oeis.org/A304681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the minimal odd positive integer t such that the * first
 *            t*n bits of the Thue-Morse sequence form an n-antipower * (i.e.,
 *            the n consecutive blocks of length t are all distinct)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Validated: partial, pass, 79 terms through n = 79 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static int cmp_ulong(const void *a, const void *b)
{
    unsigned long long ua = *(const unsigned long long *)a;
    unsigned long long ub = *(const unsigned long long *)b;
    return (ua < ub) ? -1 : (ua > ub);
}

/* Helper: hash a block of length t starting at position start.
 * Uses a simple 64‑bit rolling hash with base 131. */
static unsigned long long
hash_block(idx_t start, idx_t t)
{
    unsigned long long h = 0;
    idx_t i;
    for (i = 0; i < t; ++i) {
        idx_t pos = start + i;
        /* Parity of popcount gives Thue-Morse bit (0 for even, 1 for odd). */
        unsigned bit = __builtin_parityll(pos);
        h = h * 131ULL + bit;
    }
    return h;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined but harmless */
    idx_t t = 1;                   /* first odd candidate */
    while (true) {
        /* Allocate array for n hashes. */
        unsigned long long *h = (unsigned long long *)malloc(sizeof(unsigned long long) * (size_t)n);
        if (!h) exit(1);           /* abort on allocation failure */
        idx_t i;
        for (i = 0; i < n; ++i) {
            h[i] = hash_block(i * t, t);
        }
        /* Check pairwise distinctness via sorting. */
        qsort(h, (size_t)n, sizeof(unsigned long long), cmp_ulong);
        bool unique = true;
        for (i = 1; i < n; ++i) {
            if (h[i] == h[i - 1]) { unique = false; break; }
        }
        free(h);
        if (unique) return (seq_t)t;
        t += 2;   /* next odd integer */
    }
}
