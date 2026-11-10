/*
 * A352710  Least k such that A352483(k) = 2*n-1, or -1 if no such k exists.
 * https://oeis.org/A352710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A351913(2*n-1) = least k with A352483(k) = 2*n-1, else
 *            -1; computed by a search over k with a sieve of d(k)
 * Range:     u8 n <= 16, u16 n <= 646, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9631 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A352710_LIM (1u << 24)

static uint16_t *A352710_d = NULL;

static void A352710_init(void)
{
    uint32_t L = A352710_LIM;
    uint8_t *e = (uint8_t *)calloc(L + 1, 1);
    uint32_t *pr = (uint32_t *)malloc(sizeof(uint32_t) * 1100000);
    uint32_t np = 0, i, j;
    A352710_d = (uint16_t *)calloc(L + 1, sizeof(uint16_t));
    A352710_d[1] = 1;
    for (i = 2; i <= L; i++) {
        if (A352710_d[i] == 0) {
            A352710_d[i] = 2;
            e[i] = 1;
            pr[np++] = i;
        }
        for (j = 0; j < np; j++) {
            uint64_t ip = (uint64_t)i * pr[j];
            if (ip > L) break;
            if (i % pr[j] == 0) {
                e[ip] = (uint8_t)(e[i] + 1);
                A352710_d[ip] = (uint16_t)(A352710_d[i] / (e[i] + 1) * (e[i] + 2));
                break;
            } else {
                e[ip] = 1;
                A352710_d[ip] = (uint16_t)(A352710_d[i] * 2);
            }
        }
    }
    free(e);
    free(pr);
}

static uint64_t A352710_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t r = a % b; a = b; b = r; }
    return a;
}

seq_t a(idx_t n)
{
    uint64_t m = 2 * (uint64_t)n - 1;
    uint32_t k;
    bool filter = (m < 0xFFFFFFFFULL);
    if (A352710_d == NULL) A352710_init();
    for (k = 3; k <= A352710_LIM; k++) {
        uint64_t d = A352710_d[k];
        uint64_t diff, g;
        if (k <= d) continue;
        diff = k - d;
        if (filter && diff > m * d * d) continue;
        g = A352710_gcd(diff, (uint64_t)k * d);
        if (diff / g == m) return (seq_t)k;
    }
    return (seq_t)0 - (seq_t)1;
}
