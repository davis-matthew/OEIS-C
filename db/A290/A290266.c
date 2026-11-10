/*
 * A290266  Distinct terms in A007944.
 * https://oeis.org/A290266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th distinct value of A007944(m), m = 1,2,...;
 *            A007944(m) = largest even k with 6,8,...,k all sums of 2 of the
 *            first m odd primes (search/sieve)
 * Range:     u8 n <= 25, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static bool A290266_is_odd_prime(uint64_t num) {
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

static uint64_t A290266_A007944(uint64_t n) {
    if (n == 0) return 0;
    uint64_t *primes = (uint64_t *)malloc((size_t)n * sizeof(uint64_t));
    if (primes == NULL) return 0;
    uint64_t count = 0, candidate = 3;
    while (count < n) {
        if (A290266_is_odd_prime(candidate)) {
            primes[count++] = candidate;
        }
        candidate += 2;
    }
    uint64_t max_result = 2 * primes[n - 1];
    bool *ach = (bool *)calloc((size_t)(max_result + 1), sizeof(bool));
    if (ach == NULL) { free(primes); return 0; }
    for (uint64_t i = 0; i < n; i++) {
        for (uint64_t j = i; j < n; j++) {
            uint64_t s = primes[i] + primes[j];
            if (s <= max_result) ach[s] = true;
        }
    }
    uint64_t result = 4;
    for (uint64_t k = 6; k <= max_result; k += 2) {
        if (ach[k]) result = k; else break;
    }
    free(primes);
    free(ach);
    return result;
}

seq_t a(idx_t n) {
    uint64_t last = 0;
    idx_t cnt = 0;
    for (uint64_t m = 1;; m++) {
        uint64_t v = A290266_A007944(m);
        if (v != last) {
            last = v;
            cnt++;
            if (cnt == n) return (seq_t)v;
        }
    }
}
