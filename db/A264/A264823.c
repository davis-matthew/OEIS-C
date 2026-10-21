/*
 * A264823  Centered 16-gonal (or hexadecagonal) primes.
 * https://oeis.org/A264823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime of form 8*k^2 - 8*k + 1 (k >= 2)
 * Range:     u8 n <= 3, u16 n <= 37, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Deterministic Miller–Rabin primality test for 64‑bit unsigned integers. */
static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;
    static const uint32_t smallPrimes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37
    };
    for (size_t i = 0; i < sizeof(smallPrimes) / sizeof(smallPrimes[0]); ++i) {
        uint32_t p = smallPrimes[i];
        if (n % p == 0)
            return n == p;
    }

    uint64_t d = n - 1;
    uint32_t s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* Bases for deterministic Miller–Rabin up to 2^64. */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;

        __uint128_t x = 1, base = a;
        uint64_t r = d;
        while (r) {
            if (r & 1)
                x = (x * base) % n;
            base = (base * base) % n;
            r >>= 1;
        }

        if (x == 1 || x == n - 1)
            continue;
        bool composite = true;
        for (uint32_t j = 1; j < s; ++j) {
            x = (x * x) % n;
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* Cached sequence storage. */
static seq_t *seq_arr = NULL;     /* array of computed terms */
static size_t seq_cap = 0;        /* allocated capacity (in elements) */
static size_t seq_len = 0;        /* number of terms stored */
static idx_t next_k = 2;          /* next k candidate to test */

/* Allocate or grow the cache array to hold at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap == 0 ? 16 : seq_cap * 2;
    if (new_cap < need)
        new_cap = need;
    seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n. */
    while (seq_len < n) {
        /* Compute p = 8*k^2 - 8*k + 1 using 128‑bit to avoid overflow. */
        unsigned __int128 k = next_k;
        unsigned __int128 val = 8 * k * k - 8 * k + 1;
        uint64_t val64 = (uint64_t)val;          /* value fits in 64‑bit for n ≤ 10000 */

        if (isPrime64(val64)) {
            ensure_capacity(seq_len + 1);
            seq_arr[seq_len++] = (seq_t)val64;
        }
        ++next_k;
    }
    return seq_arr[n - 1];
}
