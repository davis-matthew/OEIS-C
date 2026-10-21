/*
 * A247947  Four-digit odd semiprimes with all digits distinct.
 * https://oeis.org/A247947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 863, u32 n <= 863, u64 n <= 863, u128 n <= 863,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements sequence A247947: four‑digit odd semiprimes with all distinct digits. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Smallest prime factor table up to 9999 */
static uint64_t spf[10000];
static bool spf_ready = false;

static void init_spf(void)
{
    if (spf_ready) return;
    for (size_t i = 0; i <= 9999; ++i) spf[i] = i;
    spf[0] = 0;
    spf[1] = 1;
    for (size_t i = 2; i * i <= 9999; ++i) {
        if (spf[i] == i) {  /* i is prime */
            for (size_t j = i * i; j <= 9999; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }
    spf_ready = true;
}

static bool distinct_digits(uint64_t n)
{
    bool seen[10] = {false};
    for (int k = 0; k < 4; ++k) {
        unsigned d = n % 10;
        if (seen[d]) return false;
        seen[d] = true;
        n /= 10;
    }
    return true;
}

/* Cache of the sequence */
static seq_t cache[1000];
static size_t cache_len = 0;
static bool cache_ready = false;

static void init_cache(void)
{
    if (cache_ready) return;
    init_spf();
    for (int num = 1001; num <= 9999; num += 2) {
        if (!distinct_digits((uint64_t)num)) continue;
        uint64_t p = spf[(size_t)num];
        if (p == (uint64_t)num) continue;           /* prime */
        uint64_t q = (uint64_t)num / p;
        if (spf[(size_t)q] != q) continue;          /* not a semiprime */
        cache[cache_len++] = (seq_t)num;
        if (cache_len >= 1000) break;               /* safety */
    }
    cache_ready = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (!cache_ready) init_cache();
    if ((idx_t)n <= cache_len) return cache[n - 1];
    return 0;
}
