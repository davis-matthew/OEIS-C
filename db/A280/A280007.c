/*
 * A280007  Balanced single (or balanced isolated) numbers (of order one):
 *          single (or isolated) numbers which are the average of the previous
 *          single (or isolated) number and the following single (or isolated)
 *          number.
 * https://oeis.org/A280007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A167706(k) where A167706(k) = (A167706(k-1) +
 *            A167706(k+1))/2, k >= 2; computed by sieve of primes and scan of
 *            single numbers
 * Range:     u8 n <= 6, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *A280007_cache = NULL;
static size_t A280007_cnt = 0;

static void A280007_build(uint64_t bound)
{
    uint64_t size = bound + 4;
    uint8_t *comp = (uint8_t *)calloc((size_t)size, 1);
    if (!comp) return;
    comp[0] = comp[1] = 1;
    for (uint64_t i = 2; i * i < size; i++) {
        if (!comp[i]) {
            for (uint64_t j = i * i; j < size; j += i) comp[j] = 1;
        }
    }
    free(A280007_cache);
    A280007_cache = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)(bound / 8 + 16));
    A280007_cnt = 0;
    if (!A280007_cache) { free(comp); return; }

    uint64_t prev = 0, cur = 0;
    int have = 0;
    for (uint64_t x = 2; x <= bound; x++) {
        bool single = false;
        if (!comp[x]) {
            bool twin = (x > 2 && !comp[x - 2]) || !comp[x + 2];
            single = !twin;
        } else if (x >= 4 && !comp[x - 1] && !comp[x + 1]) {
            single = true;
        }
        if (!single) continue;
        if (have >= 2 && 2 * cur == prev + x) {
            A280007_cache[A280007_cnt++] = cur;
        }
        prev = cur;
        cur = x;
        if (have < 2) have++;
    }
    free(comp);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    uint64_t bound = 1024;
    while (A280007_cnt < (size_t)n) {
        A280007_build(bound);
        if (!A280007_cache) return (seq_t)0;
        bound <<= 1;
    }
    return (seq_t)A280007_cache[n - 1];
}
