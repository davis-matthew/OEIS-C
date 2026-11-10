/*
 * A336471  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A329697(i) = A329697(j) and A336158(i) = A336158(j), for all i,
 *          j >= 1.
 * https://oeis.org/A336471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of the pair
 *            [A329697(n), A336158(n)]: a(n) = 1 + (number of distinct pairs
 *            first appearing before the first occurrence of n's pair);
 *            computed by a running scan over 1..n with memoised labels
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8332 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    uint64_t c;      /* A329697 value */
    uint8_t e[16];   /* sorted-descending exponents of odd part (signature of A336158) */
} rgs336471_key;

static rgs336471_key *rgs336471_keys = NULL;
static size_t rgs336471_nkeys = 0, rgs336471_capkeys = 0;
static unsigned *rgs336471_lab = NULL;
static uint64_t rgs336471_done = 0, rgs336471_caplab = 0;

static uint64_t rgs336471_lpf(uint64_t m)
{
    uint64_t largest = 1;
    while ((m & 1ULL) == 0) { largest = 2; m >>= 1; }
    for (uint64_t f = 3; f <= m / f; f += 2) {
        if (m % f == 0) {
            largest = f;
            while (m % f == 0) m /= f;
        }
    }
    if (m > 1) largest = m;
    return largest;
}

static uint64_t rgs336471_A329697(uint64_t n)
{
    uint64_t cnt = 0;
    while (n > 2 && (n & (n - 1)) != 0) {
        while ((n & 1ULL) == 0) n >>= 1;
        if (n == 1) break;
        uint64_t p = rgs336471_lpf(n);
        n = n - n / p;
        cnt++;
    }
    return cnt;
}

static void rgs336471_sig(uint64_t n, uint8_t *e)
{
    int k = 0;
    while ((n & 1ULL) == 0) n >>= 1;
    for (uint64_t f = 3; f <= n / f; f += 2) {
        if (n % f == 0) {
            int x = 0;
            while (n % f == 0) { n /= f; x++; }
            e[k++] = (uint8_t)x;
        }
    }
    if (n > 1) e[k++] = 1;
    for (int i = 0; i < k; i++)
        for (int j = i + 1; j < k; j++)
            if (e[i] < e[j]) { uint8_t t = e[i]; e[i] = e[j]; e[j] = t; }
}

seq_t a(idx_t n)
{
    if (n > rgs336471_done) {
        if (n + 1 > rgs336471_caplab) {
            uint64_t nc = (uint64_t)n * 2 + 16;
            rgs336471_lab = (unsigned *)realloc(rgs336471_lab, nc * sizeof(unsigned));
            rgs336471_caplab = nc;
        }
        for (uint64_t i = rgs336471_done + 1; i <= (uint64_t)n; i++) {
            rgs336471_key k;
            memset(&k, 0, sizeof k);
            k.c = rgs336471_A329697(i);
            rgs336471_sig(i, k.e);
            size_t j;
            for (j = 0; j < rgs336471_nkeys; j++)
                if (memcmp(&rgs336471_keys[j], &k, sizeof k) == 0) break;
            if (j == rgs336471_nkeys) {
                if (rgs336471_nkeys == rgs336471_capkeys) {
                    rgs336471_capkeys = rgs336471_capkeys ? rgs336471_capkeys * 2 : 64;
                    rgs336471_keys = (rgs336471_key *)realloc(rgs336471_keys,
                                        rgs336471_capkeys * sizeof(rgs336471_key));
                }
                rgs336471_keys[rgs336471_nkeys++] = k;
            }
            rgs336471_lab[i] = (unsigned)(j + 1);
        }
        rgs336471_done = n;
    }
    return (seq_t)rgs336471_lab[n];
}
