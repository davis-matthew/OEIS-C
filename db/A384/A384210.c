/*
 * A384210  Number of numbers <= n of the form p * m^2, where p is a prime and
 *          m is an integer >= 1.
 * https://oeis.org/A384210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 719, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A384210: numbers ≤ n of the form p * m² with p prime.
   Formula: a(n) = Σ_{p ≤ n} ⌊√(n / p)⌋
   (sum over all primes p not exceeding n) */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- Utility: integer square‑root (floor) ---------- */
static inline idx_t _floor_isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        /* mid*mid cannot overflow for n ≤ 10000 (tested range) */
        if (mid * mid <= n) {
            ans = mid;
            lo  = mid + 1;
        } else {
            if (mid == 0) break;   /* avoid infinite loop for n = 0 */
            hi = mid - 1;
        }
    }
    return ans;
}

/* ---------- Main function implementing the sequence ---------- */
seq_t a(idx_t n)
{
    if (n < 2) return 0;            /* no prime ≤ 1 → result 0 */

    /* Simple sieve of Eratosthenes up to n.
       Allocate an array of bytes: 0 = composite, 1 = potential prime.   */
    uint8_t *is_prime = (uint8_t*)malloc((size_t)(n + 1));
    if (!is_prime) return 0;        /* allocation failure – return 0 */

    /* Initialize: 0 and 1 are not prime, others true initially. */
    is_prime[0] = 0; is_prime[1] = 0;
    for (idx_t i = 2; i <= n; ++i) is_prime[i] = 1;

    /* Mark composites. */
    for (idx_t p = 2; p * p <= n; ++p) {
        if (is_prime[p]) {
            for (idx_t m = p * p; m <= n; m += p) {
                is_prime[m] = 0;
            }
        }
    }

    /* Accumulate the sum of floor(sqrt(n/p)) for each prime p. */
    seq_t sum = 0;
    for (idx_t p = 2; p <= n; ++p) {
        if (is_prime[p]) {
            idx_t div = n / p;
            idx_t root = _floor_isqrt_u64(div);
            sum += (seq_t)root;
        }
    }

    free(is_prime);
    return sum;
}
