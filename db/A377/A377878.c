/*
 * A377878  Numbers k for which A276085(k) is a multiple of 3125, where
 *          A276085 is fully additive with a(p) = p#/p.
 * https://oeis.org/A377878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A276085(k) == 0 (mod 5^5); A276085 is
 *            fully additive with a(p) = (product of primes < p) mod 3125,
 *            computed by a linear sieve over increasing limits
 * Range:     u8 n <= 1, u16 n <= 21, u32 n <= 15711, u64 n <= 15711,
 *            u128 n <= 15711, big any n
 * Validated: standard, pass, 62 terms through n = 9939 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static uint64_t *terms_arr = NULL;
static size_t terms_cnt = 0;
static uint64_t sieved_upto = 0;

static void run_sieve(uint64_t N)
{
    uint16_t *f = (uint16_t *)calloc((size_t)N + 1, sizeof(uint16_t));
    unsigned char *comp = (unsigned char *)calloc((size_t)N + 1, 1);
    size_t pcap = 1024, pcnt = 0;
    uint32_t *pr = (uint32_t *)malloc(pcap * sizeof(uint32_t));
    uint16_t *wt = (uint16_t *)malloc(pcap * sizeof(uint16_t));
    uint32_t prod = 1; /* product of primes so far mod 3125 */
    size_t tcap = 1024;
    uint64_t *t = (uint64_t *)malloc(tcap * sizeof(uint64_t));
    size_t tc = 0;

    for (uint64_t i = 1; i <= N; i++) {
        if (i >= 2 && !comp[i]) {
            if (pcnt == pcap) {
                pcap *= 2;
                pr = (uint32_t *)realloc(pr, pcap * sizeof(uint32_t));
                wt = (uint16_t *)realloc(wt, pcap * sizeof(uint16_t));
            }
            pr[pcnt] = (uint32_t)i;
            wt[pcnt] = (uint16_t)prod;
            pcnt++;
            f[i] = (uint16_t)prod;
            prod = (uint32_t)((prod * (i % 3125)) % 3125);
        }
        if (f[i] == 0) {
            if (tc == tcap) {
                tcap *= 2;
                t = (uint64_t *)realloc(t, tcap * sizeof(uint64_t));
            }
            t[tc++] = i;
        }
        for (size_t j = 0; j < pcnt; j++) {
            uint64_t m = i * pr[j];
            if (m > N) break;
            comp[m] = 1;
            f[m] = (uint16_t)((f[i] + wt[j]) % 3125);
            if (i % pr[j] == 0) break;
        }
    }
    free(f);
    free(comp);
    free(pr);
    free(wt);
    free(terms_arr);
    terms_arr = t;
    terms_cnt = tc;
    sieved_upto = N;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    while (terms_cnt < (size_t)n) {
        uint64_t N = sieved_upto ? sieved_upto * 4 : (1u << 18);
        run_sieve(N);
    }
    return (seq_t)terms_arr[(size_t)n - 1];
}
