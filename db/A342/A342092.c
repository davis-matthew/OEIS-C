/*
 * A342092  Odd numbers k such that if k = A001065(m) for some m then m is a
 *          squarefree semiprime (A006881).
 * https://oeis.org/A342092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th odd k such that every m with A001065(m) = k is a
 *            squarefree semiprime; computed by a sieve over m = P^e*j (P
 *            largest prime factor), marking every k reached by a
 *            non-squarefree-semiprime m
 * Range:     u8 n <= 71, u16 n <= 5931, u32 n <= 8251, u64 n <= 8251,
 *            u128 n <= 8251, big any n
 * Validated: standard, pass, 75 terms through n = 8026 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static uint64_t *A342092_res = NULL;
static idx_t A342092_count = 0;
static uint64_t A342092_limit = 0;

static void A342092_build(uint64_t L) {
    uint64_t *sig = (uint64_t *)calloc((size_t)(L + 1), sizeof(uint64_t));
    uint32_t *gpf = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    unsigned char *touched = (unsigned char *)calloc((size_t)(L + 1), 1);
    uint32_t *primes = (uint32_t *)malloc((size_t)(L + 1) * sizeof(uint32_t));
    uint64_t np = 0, i, k;
    for (i = 1; i <= L; i++)
        for (k = i; k <= L; k += i) sig[k] += i;
    gpf[1] = 1;
    for (i = 2; i <= L; i++) {
        if (gpf[i] == 0) {
            primes[np++] = (uint32_t)i;
            for (k = i; k <= L; k += i) gpf[k] = (uint32_t)i;
        }
    }
    if (L >= 1) touched[1] = 1; /* primes m give 1 */
    for (uint64_t j = 1; j <= L; j++) {
        uint64_t g = gpf[j];
        uint64_t lo = 0, hi = np;
        while (lo < hi) {
            uint64_t mid = (lo + hi) / 2;
            if (primes[mid] <= g) lo = mid + 1; else hi = mid;
        }
        uint64_t start = lo;
        /* e = 1, j composite (j prime gives squarefree semiprimes) */
        if (j >= 2 && gpf[j] != j && sig[j] < L) {
            uint64_t sj = sig[j] - j;
            for (uint64_t t = start; t < np; t++) {
                uint64_t P = primes[t];
                uint64_t s = P * sj + sig[j];
                if (s > L) break;
                touched[s] = 1;
            }
        }
        /* e >= 2 */
        for (uint64_t t = start; t < np; t++) {
            uint64_t P = primes[t];
            if (P * j > L) break;
            uint64_t pprev = P;
            uint64_t pe = P * P;
            while (pprev * j <= L) {
                uint64_t sp = (pe * P - 1) / (P - 1);
                uint64_t s = sp * sig[j] - pe * j;
                if (s <= L) touched[s] = 1;
                pprev = pe;
                pe *= P;
            }
        }
    }
    free(A342092_res);
    A342092_res = (uint64_t *)malloc((size_t)(L + 1) * sizeof(uint64_t));
    A342092_count = 0;
    for (i = 1; i <= L; i += 2)
        if (!touched[i]) A342092_res[A342092_count++] = i;
    A342092_limit = L;
    free(sig); free(gpf); free(touched); free(primes);
}

seq_t a(idx_t n) {
    uint64_t L = A342092_limit ? A342092_limit : 4096;
    if (A342092_res == NULL) A342092_build(L);
    while (A342092_count < n) {
        L *= 2;
        A342092_build(L);
    }
    return (seq_t)A342092_res[n - 1];
}
