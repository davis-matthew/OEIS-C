/*
 * A327742  Triangle T(k, n) read by rows, where the entries of the triangle
 *          are lengths of longest runs of consecutive sums of k-length
 *          combinations of first n primes. Specially, T(0, n) = 1.
 * https://oeis.org/A327742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(k,r) where r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r*(r+1)/2. T(k,r) is the length of the longest consecutive run
 *            of sums of all k‑combinations of the first r primes
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   longest_run
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers needed for both C11 and C++17 */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/longest_run.h"

/* ---------- constants ---------------------------------------- */
#define MAX_PRIMES   200            /* enough for n ≤ 10000  (r≈142) */
#define MAX_WORDS    1024           /* enough for sum of first 200 primes */

/* ---------- global data -------------------------------------- */
static uint32_t primes[MAX_PRIMES];
static size_t primes_cnt = 0;

/* -----------------------------------------------------------------
   simple primality test for small numbers
   ----------------------------------------------------------------- */
static bool is_small_prime(uint32_t n)
{
    if (n < 2) return false;
    for (uint32_t i = 2; (uint32_t)((long long)i * i) <= n; ++i)
        if (n % i == 0) return false;
    return true;
}

/* -----------------------------------------------------------------
   Generate enough primes until we have `need` primes.
   The function is idempotent and safe to call repeatedly.
   ----------------------------------------------------------------- */
static void ensure_primes(size_t need)
{
    if (primes_cnt >= need) return;
    uint32_t cand = (primes_cnt == 0) ? 2 : primes[primes_cnt - 1] + 1;
    while (primes_cnt < need) {
        while (!is_small_prime(cand)) ++cand;
        primes[primes_cnt++] = cand;
        ++cand;
    }
}

/* -----------------------------------------------------------------
   Shift the bitset src left by `shift` bits and OR into dest.
   Both arrays are assumed to have `words` 64‑bit words.
   ----------------------------------------------------------------- */
static void shift_or(uint64_t *dest, const uint64_t *src,
                     int shift, int words)
{
    int wshift = shift >> 6;          /* whole words */
    int bshift = shift & 63;          /* remaining bits */

    if (bshift == 0) {
        for (int i = words - 1; i >= wshift; --i)
            dest[i] |= src[i - wshift];
    } else {
        for (int i = words - 1; i > wshift; --i)
            dest[i] |= (src[i - wshift] << bshift) |
                       (src[i - wshift - 1] >> (64 - bshift));
        dest[wshift] |= src[0] << bshift;
    }
}

/* -----------------------------------------------------------------
   Compute T(k,r) for a single pair (k,r) and return it as seq_t.
   ----------------------------------------------------------------- */
static seq_t compute_T(idx_t r, idx_t k)
{
    /* make sure we have the first r primes */
    ensure_primes(r);

    /* upper bound for sums: sum of first r primes */
    uint32_t sum = 0;
    for (size_t i = 0; i < r; ++i) sum += primes[i];
    int max_bit = (int)sum;
    int words   = (max_bit >> 6) + 1;

    /* bitset DP: dp[0..r][0..words-1] */
    static uint64_t dp[MAX_PRIMES + 1][MAX_WORDS];
    for (size_t kk = 0; kk <= r; ++kk)
        memset(dp[kk], 0, words * sizeof(uint64_t));

    dp[0][0] = 1ULL;   /* sum 0 with 0 primes */

    /* process each prime */
    for (size_t pi = 0; pi < r; ++pi) {
        uint32_t p = primes[pi];
        for (int kk = (int)r; kk >= 1; --kk)
            shift_or(dp[kk], dp[kk - 1], (int)p, words);
    }

    /* longest run for exactly k primes */
    seq_t best = longest_run(dp[k], words, max_bit);
    return (seq_t)best;   /* cast works for integral and arbitrary‑precision seq_t */
}

/* -----------------------------------------------------------------
   Main function required by the checker
   ----------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* find row r and column k such that n = r*(r+1)/2 + k   (0‑based) */
    idx_t r = 0;
    while ((idx_t)(r + 1) * (r + 2) / 2 <= n)
        ++r;
    idx_t k = n - r * (r + 1) / 2;

    return compute_T(r, k);
}
