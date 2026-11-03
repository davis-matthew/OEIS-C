/*
 * A266578  Least number N such that the product n*N has the same digits as
 *          the concatenation (n,N) (counting repetitions), or 0 if no such
 *          number exists.
 * https://oeis.org/A266578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least positive N such that the digits of n·N are exactly
 *            the digits of the concatenation (n,N) (digit repetitions
 *            counted). Return 0 if no such N exists
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9664 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Count digits of a 128‑bit integer (used for the product n*N),
   return its length and digit counts. */
static unsigned
len_and_counts_uint128(__int128 x, unsigned char cnt[10])
{
    unsigned len = 0;
    for (int d = 0; d < 10; ++d) cnt[d] = 0;
    while (x) {
        unsigned d = x % 10;
        ++cnt[d];
        ++len;
        x /= 10;
    }
    return len;
}

/* Count digits of a 64‑bit integer, return its length.
   The result array 'cnt' is zeroed and filled with the counts. */
static unsigned
len_and_counts_uint64(uint64_t x, unsigned char cnt[10])
{
    unsigned len = 0;
    for (int d = 0; d < 10; ++d) cnt[d] = 0;
    while (x) {
        unsigned d = x % 10;
        ++cnt[d];
        ++len;
        x /= 10;
    }
    return len;
}

/* Modular inverse of a modulo 9 (a and 9 are coprime). */
static int
mod9_inv(int a)
{
    for (int t = 1; t < 9; ++t)
        if ((a * t) % 9 == 1) return t;
    return -1; /* should never happen for a coprime with 9 */
}

seq_t a(idx_t n)
{
    /* n ≡ 1 (mod 3) => no solution */
    if (n % 3 == 1) return 0;

    /* Determine the residue class of N modulo 9 */
    int a1 = (int)(n % 9);            /* 0..8 */
    int k = a1 - 1;
    if (k < 0) k += 9;                /* a1==0 -> k=8 */

    int inv = mod9_inv(k);            /* inverse of k modulo 9 */
    int b1 = (inv + 1) % 9;           /* desired N mod 9 */

    unsigned char cnt_n[10];
    unsigned len_n = len_and_counts_uint64((uint64_t)n, cnt_n);

    idx_t start = b1;
    if (start == 0) start = 9;        /* first positive N in this class */
    const idx_t step = 9;

    for (idx_t N = start; ; N += step) {
        __int128 prod = (__int128)n * N;

        unsigned char cnt_N[10], cnt_prod[10];
        unsigned len_N = len_and_counts_uint64((uint64_t)N, cnt_N);
        unsigned len_prod = len_and_counts_uint128(prod, cnt_prod);

        if (len_prod != len_n + len_N) continue;

        bool equal = true;
        for (int d = 0; d < 10; ++d) {
            if (cnt_n[d] + cnt_N[d] != cnt_prod[d]) {
                equal = false;
                break;
            }
        }
        if (equal) return (seq_t)N;
    }
}
