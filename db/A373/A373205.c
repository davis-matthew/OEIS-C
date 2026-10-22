/*
 * A373205  Numbers m such that m^m == m (mod 10^(len(m) + 1)), where len(m)
 *          is the number of digits of m (A055642).
 * https://oeis.org/A373205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 21, u32 n <= 22, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A373205:
 * a(n) is the n‑th positive integer m such that
 *          m^m ≡ m  (mod 10^(len(m)+1)),
 * where len(m) is the number of decimal digits of m.
 * The sequence starts 1,51,57,101,…
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- helper functions --------------------------------------- */

/* number of decimal digits of x (x > 0) */
static int digit_len(seq_t x)
{
    if (x == 0) return 1;
    int c = 0;
    while (x > 0) {
        x /= 10;
        ++c;
    }
    return c;
}

/* 10^e for non‑negative integer e */
static seq_t pow10(idx_t e)
{
    seq_t r = 1;
    for (idx_t i = 0; i < e; ++i) r *= 10;
    return r;
}

/* modular exponentiation: base^exp mod mod */
static seq_t mod_pow(seq_t base, idx_t exp, seq_t mod)
{
    seq_t result = 1 % mod;
    seq_t b = base % mod;
    while (exp > 0) {
        if (exp & 1) result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return result;
}

/* ---------- static cache ------------------------------------------ */

/* maximum value that fits in seq_t */
static const idx_t MAX_SEQ_T = (idx_t)((seq_t)-1);

/* cache of computed terms */
static seq_t *cache      = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static uint64_t last_checked = 0;   /* last integer that has been tested */

/* grow cache if necessary */
static void cache_grow(void)
{
    if (cache_cap < 16) cache_cap = 16;
    else cache_cap <<= 1;
    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
}

/* ---------- main function ---------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence is 1‑based; undefined for 0 */

    /* lazy initialization of cache */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 1;              /* a(1) = 1 */
        cache_size = 1;
        last_checked = 1;
    }

    /* generate terms until we have at least n of them */
    while (cache_size < n) {
        uint64_t m = last_checked + 1;
        while (1) {
            /* safety: m must fit into seq_t */
            if ((idx_t)m > MAX_SEQ_T) {
                /* cannot store more terms; abort gracefully */
                return 0;
            }

            int d = digit_len((seq_t)m);
            seq_t mod = pow10((idx_t)d + 1);
            seq_t rem = (seq_t)m % mod;
            seq_t pow = mod_pow((seq_t)m, (idx_t)m, mod);

            if (pow == rem) {
                /* store the term */
                if (cache_size >= cache_cap) cache_grow();
                cache[cache_size++] = (seq_t)m;
                last_checked = m;
                break;   /* found next term, go on to the next one */
            }
            ++m;
        }
    }

    return cache[n - 1];
}
