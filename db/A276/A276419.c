/*
 * A276419  Non-Fibonacci numbers n such that A129761(n) = 1.
 * https://oeis.org/A276419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th non-Fibonacci m >= 1 with A129761(m) = 1, where
 *            A129761(m) = Fibbinary(m+1) - Fibbinary(m) and Fibbinary(m) is
 *            the Zeckendorf representation of m read as binary (search with
 *            memoization)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A276419_fibs[100];
static int A276419_nf = 0;

static void A276419_init_fibs(void)
{
    if (A276419_nf) return;
    A276419_fibs[0] = 1;
    A276419_fibs[1] = 2;
    A276419_nf = 2;
    while (A276419_nf < 100) {
        uint64_t x = A276419_fibs[A276419_nf - 1];
        uint64_t y = A276419_fibs[A276419_nf - 2];
        if (x > UINT64_MAX - y) break;
        A276419_fibs[A276419_nf++] = x + y;
    }
}

/* Zeckendorf bits of m as a binary pattern (Fibbinary number); terms = number of summands */
static unsigned __int128 A276419_zeck(uint64_t m, int *terms)
{
    unsigned __int128 bits = 0;
    int t = 0;
    int i;
    for (i = A276419_nf - 1; i >= 0; i--) {
        if (A276419_fibs[i] <= m) {
            m -= A276419_fibs[i];
            bits |= ((unsigned __int128)1) << i;
            t++;
        }
    }
    *terms = t;
    return bits;
}

static uint64_t *A276419_cache = NULL;
static size_t A276419_size = 0, A276419_cap = 0;
static uint64_t A276419_next = 1;

seq_t a(idx_t n)
{
    A276419_init_fibs();
    while (A276419_size < (size_t)n) {
        int t1, t2;
        uint64_t m = A276419_next++;
        unsigned __int128 f1 = A276419_zeck(m, &t1);
        unsigned __int128 f2 = A276419_zeck(m + 1, &t2);
        if (t1 != 1 && f2 - f1 == 1) {
            if (A276419_size == A276419_cap) {
                A276419_cap = A276419_cap ? A276419_cap * 2 : 1024;
                A276419_cache = (uint64_t *)realloc(A276419_cache, A276419_cap * sizeof(uint64_t));
            }
            A276419_cache[A276419_size++] = m;
        }
    }
    return (seq_t)A276419_cache[(size_t)n - 1];
}
