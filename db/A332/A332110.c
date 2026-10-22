/*
 * A332110  Numbers that are not the sum of ten (10) positive cubes.
 * https://oeis.org/A332110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A332110: numbers that are not the sum of ten positive cubes. */

#include <stdbool.h>
#include <stddef.h>

static seq_t cache[100];   /* up to 99 terms + sentinel */
static size_t cache_len = 0;
static bool initialized = false;

static void build_cache(void)
{
    if (initialized) return;

    /* Positive cubes up to 374 (7³ = 343, 8³ = 512 > 374). */
    const int cubes[7] = {1, 8, 27, 64, 125, 216, 343};
    const int MAX_SUM = 374;
    const int MAX_K   = 10;          /* exactly ten cubes */

    /* dp[k][s] == true if sum s can be expressed as a sum of exactly k cubes. */
    static bool dp[11][375] = {0};

    dp[0][0] = true;
    for (int k = 1; k <= MAX_K; ++k) {
        for (int s = 1; s <= MAX_SUM; ++s) {
            bool ok = false;
            for (int i = 0; i < 7; ++i) {
                int v = cubes[i];
                if (s >= v && dp[k-1][s - v]) {
                    ok = true;
                    break;
                }
            }
            dp[k][s] = ok;
        }
    }

    /* Collect numbers that cannot be written as a sum of ten positive cubes. */
    size_t count = 0;
    for (int s = 1; s <= MAX_SUM; ++s) {
        if (!dp[MAX_K][s]) {
            cache[count++] = (seq_t) s;  /* cast to seq_t; value fits 16‑bit or larger */
        }
    }
    cache_len = count;
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below offset */

    build_cache();

    if ((size_t) n <= cache_len)
        return cache[(size_t)(n - 1)];
    return 0;                      /* sequence has only 99 terms */
}
