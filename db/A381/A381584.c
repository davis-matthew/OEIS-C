/*
 * A381584  Starts of runs of 4 consecutive integers that are all terms in
 *          A381581.
 * https://oeis.org/A381584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search. a(n) = n-th k such that A291711(k+i) divides k+i for i =
 *            0..3, where A291711 is the greedy coin count with the coin
 *            values Fibonacci(2j) = 1, 3, 8, 21, 55, ... The scan checks k+3
 *            first and then skips ahead, and it uses a lookup table for
 *            counts below Fibonacci(32)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 50 terms through n = 981 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define A381584_LOWIDX 15   /* coins[15] = F(32) = 2178309 */

static uint64_t A381584_coins[64];
static int A381584_ncoins = 0;
static unsigned char *A381584_T = NULL;
static uint64_t A381584_B = 0;
static int A381584_top = 0;

static void A381584_init(void)
{
    if (A381584_ncoins) return;
    uint64_t p = 1, q = 3;
    A381584_coins[A381584_ncoins++] = p;
    A381584_coins[A381584_ncoins++] = q;
    for (;;) {
        if (q > UINT64_MAX / 3) break;
        uint64_t c = 3 * q - p;
        A381584_coins[A381584_ncoins++] = c;
        p = q;
        q = c;
        if (A381584_ncoins >= 63) break;
    }
    A381584_B = A381584_coins[A381584_LOWIDX];
    A381584_T = (unsigned char *)malloc((size_t)A381584_B);
    A381584_T[0] = 0;
    int j = 0;
    for (uint64_t r = 1; r < A381584_B; r++) {
        while (j + 1 < A381584_LOWIDX && A381584_coins[j + 1] <= r) j++;
        A381584_T[r] = (unsigned char)(A381584_T[r - A381584_coins[j]] + 1);
    }
    A381584_top = A381584_LOWIDX;
}

static uint64_t A381584_count(uint64_t x)
{
    if (x < A381584_B) return A381584_T[x];
    while (A381584_top + 1 < A381584_ncoins && A381584_coins[A381584_top + 1] <= x)
        A381584_top++;
    uint64_t rem = x, cnt = 0;
    for (int i = A381584_top; i >= A381584_LOWIDX; i--) {
        uint64_t c = A381584_coins[i];
        while (rem >= c) { rem -= c; cnt++; }
    }
    return cnt + A381584_T[rem];
}

static int A381584_good(uint64_t x)
{
    uint64_t c = A381584_count(x);
    return c != 0 && (x % c) == 0;
}

static uint64_t *A381584_terms = NULL;
static size_t A381584_cnt = 0, A381584_cap = 0;
static uint64_t A381584_s = 1;

seq_t a(idx_t n)
{
    A381584_init();
    if (n == 0) return (seq_t)0;
    while (A381584_cnt < (size_t)n) {
        uint64_t s = A381584_s;
        if (!A381584_good(s + 3)) { A381584_s = s + 4; continue; }
        if (!A381584_good(s + 2)) { A381584_s = s + 3; continue; }
        if (!A381584_good(s + 1)) { A381584_s = s + 2; continue; }
        if (!A381584_good(s))     { A381584_s = s + 1; continue; }
        if (A381584_cnt == A381584_cap) {
            size_t nc = A381584_cap ? A381584_cap * 2 : 256;
            A381584_terms = (uint64_t *)realloc(A381584_terms, nc * sizeof(uint64_t));
            A381584_cap = nc;
        }
        A381584_terms[A381584_cnt++] = s;
        A381584_s = s + 1;
    }
    return (seq_t)A381584_terms[n - 1];
}
