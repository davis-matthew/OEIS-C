/*
 * A364545  Odd numbers k such that k divides A005940(k).
 * https://oeis.org/A364545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th odd k such that k | A005940(k); computed by search,
 *            testing A005940(k) mod k via its binary-digit definition
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A364545_primes[80];
static int A364545_np = 0;

static void A364545_init(void) {
    if (A364545_np) return;
    uint64_t c = 2;
    while (A364545_np < 80) {
        bool ok = true;
        for (int j = 0; j < A364545_np; j++) {
            if (A364545_primes[j] * A364545_primes[j] > c) break;
            if (c % A364545_primes[j] == 0) { ok = false; break; }
        }
        if (ok) A364545_primes[A364545_np++] = c;
        c++;
    }
}

static bool A364545_test(uint64_t k) {
    uint64_t m = k - 1;
    int i = 1;
    uint64_t r = 1 % k;
    while (m > 0) {
        if (m & 1) {
            r = (uint64_t)(((unsigned __int128)r * (A364545_primes[i - 1] % k)) % k);
            m >>= 1;
        } else {
            m >>= 1;
            i++;
        }
    }
    return r == 0;
}

static uint64_t A364545_terms[4096];
static uint64_t A364545_count = 0;
static uint64_t A364545_next = 1;

seq_t a(idx_t n) {
    A364545_init();
    while (A364545_count < (uint64_t)n && A364545_count < 4096) {
        uint64_t k = A364545_next;
        A364545_next += 2;
        if (A364545_test(k)) A364545_terms[A364545_count++] = k;
    }
    return (seq_t)A364545_terms[n - 1];
}
