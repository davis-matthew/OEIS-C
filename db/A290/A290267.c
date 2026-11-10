/*
 * A290267  Length of runs of equal terms in A007944.
 * https://oeis.org/A290267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the n-th run of equal terms in A007944;
 *            computed by incrementally sieving sums of pairs of odd primes
 * Range:     u8 n <= 13690, u16 n <= 13690, u32 n <= 13690, u64 n <= 13690,
 *            u128 n <= 13690, big any n
 * Validated: standard, pass, 104 terms through n = 9994 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static bool A290267_is_odd_prime(uint64_t num) {
    if (num < 2) return false;
    if (num == 2) return true;
    if (num % 2 == 0) return false;
    if (num == 3) return true;
    if (num % 3 == 0) return false;
    for (uint64_t i = 5; i * i <= num; i += 6) {
        if (num % i == 0 || num % (i + 2) == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    size_t pcap = 1024, pcnt = 0;
    uint32_t *primes = (uint32_t *)malloc(pcap * sizeof(uint32_t));
    size_t acap = 1 << 16;
    unsigned char *ach = (unsigned char *)calloc(acap, 1);
    if (!primes || !ach) { free(primes); free(ach); return 0; }

    uint64_t cand = 1;
    uint64_t k = 4;          /* current A007944 value */
    uint64_t prev = 0;       /* previous value */
    uint64_t runlen = 0;
    idx_t runs = 0;
    seq_t result = 0;

    for (;;) {
        do { cand += 2; } while (!A290267_is_odd_prime(cand));
        if (pcnt == pcap) {
            pcap *= 2;
            uint32_t *np = (uint32_t *)realloc(primes, pcap * sizeof(uint32_t));
            if (!np) break;
            primes = np;
        }
        primes[pcnt++] = (uint32_t)cand;
        size_t need = (size_t)(2 * cand + 4);
        if (need > acap) {
            size_t nc = acap;
            while (nc < need) nc *= 2;
            unsigned char *na = (unsigned char *)realloc(ach, nc);
            if (!na) break;
            memset(na + acap, 0, nc - acap);
            ach = na;
            acap = nc;
        }
        for (size_t i = 0; i < pcnt; i++) ach[primes[i] + cand] = 1;
        while (ach[k + 2]) k += 2;

        if (runlen == 0) {
            prev = k; runlen = 1;
        } else if (k == prev) {
            runlen++;
        } else {
            runs++;
            if (runs == n) { result = (seq_t)runlen; break; }
            prev = k; runlen = 1;
        }
    }
    free(primes);
    free(ach);
    return result;
}
