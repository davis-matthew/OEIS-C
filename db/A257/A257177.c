/*
 * A257177  Number of partitions of n into 2 or 3 primes.
 * https://oeis.org/A257177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of unordered partitions of n into 2 or 3 primes.
 *            Computed by: - count of Goldbach partitions (unordered pairs of
 *            primes); - count of unordered triples of primes (p ≤ q ≤ r) with
 *            p+q+r = n. Complexity: O(π(n)^2) with π(n) the prime counting
 *            function
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* offset starts at 1 */

    /* ---------- sieve to get primality up to n ---------- */
    size_t lim = (size_t)n + 1;
    bool *is_prime = (bool*)calloc(lim, sizeof(bool));
    if (!is_prime) return 0;
    for (size_t i = 2; i < lim; ++i) is_prime[i] = true;
    for (idx_t i = 2; i * i <= n; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= n; j += i)
                is_prime[j] = false;

    /* ---------- collect primes into an array ---------- */
    size_t primes_cnt = 0;
    for (size_t i = 2; i <= n; ++i)
        if (is_prime[i]) ++primes_cnt;
    uint64_t *primes = (uint64_t*)malloc(primes_cnt * sizeof(uint64_t));
    if (!primes) { free(is_prime); return 0; }
    size_t pos = 0;
    for (size_t i = 2; i <= n; ++i)
        if (is_prime[i]) primes[pos++] = (uint64_t)i;

    /* ---------- count unordered partitions into 2 primes (Goldbach) ---------- */
    seq_t two_parts = 0;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > n / 2) break;
        if (is_prime[(size_t)(n - p)]) ++two_parts;   /* unordered: p ≤ n/2 */
    }

    /* ---------- count unordered partitions into 3 primes ---------- */
    seq_t three_parts = 0;
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        uint64_t p = primes[pi];
        if (p > n / 3) break;          /* p ≤ n/3 to keep p ≤ q ≤ r */
        uint64_t target = n - p;       /* q + r must equal target */

        size_t l = pi;                 /* start with q ≥ p */
        size_t r = primes_cnt ? primes_cnt - 1 : 0;   /* largest prime index */

        /* shift r to the largest index with prime ≤ target */
        while (r > pi && primes[r] > target) --r;
        if (r < l) continue;           /* no possible pair (q,r) */

        /* two‑pointer scan for unordered pairs (q,r) with q ≤ r and q+r=target */
        while (l <= r) {
            uint64_t sum = primes[l] + primes[r];
            if (sum == target) {
                ++three_parts;
                if (primes[l] == primes[r]) break;  /* q == r, only one pair */
                ++l; --r;                           /* move to next potential pair */
            } else if (sum < target) {
                ++l;
            } else {
                --r;
            }
        }
    }

    free(is_prime);
    free(primes);

    return two_parts + three_parts;
}
