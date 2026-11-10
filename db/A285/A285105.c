/*
 * A285105  Number of even terms on row n of A053632.
 * https://oeis.org/A285105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A080791(A068052(n)) = n(n+1)/2 + 1 - (number of odd
 *            coefficients of Product_{k=1..n} (1+x^k)), computed as a
 *            polynomial over GF(2) with a bitset
 * Range:     u8 n <= 30, u16 n <= 511, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 89 terms through n = 4998 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *A285105_bits = NULL;
static size_t A285105_words = 0;
static uint64_t *A285105_res = NULL;
static size_t A285105_rescap = 0;
static uint64_t A285105_cur = 0;
static int A285105_init = 0;

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    if (!A285105_init) {
        A285105_rescap = 64;
        A285105_res = (uint64_t *)malloc(A285105_rescap * sizeof(uint64_t));
        A285105_words = 1;
        A285105_bits = (uint64_t *)calloc(A285105_words, sizeof(uint64_t));
        A285105_bits[0] = 1;
        A285105_res[0] = 1;
        A285105_cur = 0;
        A285105_init = 1;
    }
    if (N > A285105_cur) {
        size_t need = (size_t)((N * (N + 1) / 2) / 64 + 2);
        if (need > A285105_words) {
            A285105_bits = (uint64_t *)realloc(A285105_bits, need * sizeof(uint64_t));
            memset(A285105_bits + A285105_words, 0, (need - A285105_words) * sizeof(uint64_t));
            A285105_words = need;
        }
        if (N + 1 > A285105_rescap) {
            size_t nc = A285105_rescap;
            while (nc < N + 1) nc <<= 1;
            A285105_res = (uint64_t *)realloc(A285105_res, nc * sizeof(uint64_t));
            A285105_rescap = nc;
        }
        for (uint64_t k = A285105_cur + 1; k <= N; ++k) {
            uint64_t dnew = k * (k + 1) / 2;
            size_t top = (size_t)(dnew / 64);
            size_t w = (size_t)(k / 64);
            unsigned b = (unsigned)(k % 64);
            uint64_t *B = A285105_bits;
            for (size_t i = top + 1; i-- > w; ) {
                uint64_t v = B[i - w] << b;
                if (b && i - w >= 1)
                    v |= B[i - w - 1] >> (64 - b);
                B[i] ^= v;
            }
            uint64_t pop = 0;
            for (size_t i = 0; i <= top; ++i)
                pop += (uint64_t)__builtin_popcountll(B[i]);
            A285105_res[k] = pop;
        }
        A285105_cur = N;
    }
    uint64_t total = N * (N + 1) / 2 + 1;
    return (seq_t)(total - A285105_res[N]);
}
