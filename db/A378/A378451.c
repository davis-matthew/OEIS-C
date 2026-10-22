/*
 * A378451  Dirichlet inverse of A119347, where A119347(n) is the number of
 *          distinct sums of distinct divisors of n.
 * https://oeis.org/A378451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; for n>1, a(n) = -∑_{d|n, d<n} A119347(n/d) * a(d)
 * Range:     i8 n <= 59, i16 n <= 4919, i32 n <= 9078, i64 n <= 9078,
 *            i128 n <= 9078, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (A119347(n) = number of distinct sums of distinct divisors of n)         */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* helpers for the cache structures                                  */
/* ------------------------------------------------------------------ */
static seq_t *seq_vals   = NULL;   /* cache for a(n)          */
static unsigned char *seq_done = NULL;
static seq_t *A119_vals   = NULL;   /* cache for A119347(n)    */
static unsigned char *A119_done = NULL;
static idx_t seq_cap = 0;

/* Ensure the cache arrays are large enough for indices up to n */
static void ensure_arrays(idx_t n)
{
    if (n < seq_cap) return;
    idx_t newcap = seq_cap ? seq_cap << 1 : 16;
    while (newcap <= n) newcap <<= 1;
    seq_vals   = (seq_t *)realloc(seq_vals,   newcap * sizeof(seq_t));
    seq_done   = (unsigned char *)realloc(seq_done,   newcap);
    A119_vals  = (seq_t *)realloc(A119_vals,  newcap * sizeof(seq_t));
    A119_done  = (unsigned char *)realloc(A119_done,  newcap);
    if (seq_cap == 0) {
        memset(seq_done,   0, newcap);
        memset(A119_done, 0, newcap);
    } else {
        memset(seq_done + seq_cap, 0, newcap - seq_cap);
        memset(A119_done + seq_cap, 0, newcap - seq_cap);
    }
    seq_cap = newcap;
}

/* ------------------------------------------------------------------ */
/* Compute A119347(k): number of distinct sums of non‑empty subsets   */
/* of distinct divisors of k                                            */
/* ------------------------------------------------------------------ */
static seq_t get_A119347(idx_t k)
{
    if (A119_done[k]) return A119_vals[k];

    /* ---------- find all divisors of k ---------- */
    idx_t cnt = 0;
    for (idx_t i = 1; i <= k / i; ++i) {
        if (k % i == 0) cnt += (i == k / i ? 1 : 2);
    }
    idx_t *divs = (idx_t *)malloc(cnt * sizeof(idx_t));
    cnt = 0;
    for (idx_t i = 1; i <= k / i; ++i) {
        if (k % i == 0) {
            divs[cnt++] = i;
            if (i != k / i) divs[cnt++] = k / i;
        }
    }

    /* ---------- compute sigma(k) ---------- */
    seq_t sigma = 0;
    for (idx_t i = 0; i < cnt; ++i) sigma += (seq_t)divs[i];

    /* Convert sigma to a 64‑bit integer for bitset sizing.  For the    */
    /* range tested (n up to ~10 000) sigma always fits in 64 bits.     */
    idx_t sigma64 = (idx_t)sigma;
    size_t words = (sigma64 >> 6) + 1;           /* number of 64‑bit words */

    /* ---------- bitset DP for subset sums ---------- */
    uint64_t *bits = (uint64_t *)calloc(words, sizeof(uint64_t));
    uint64_t *tmp  = (uint64_t *)calloc(words, sizeof(uint64_t));
    bits[0] = 1ULL;                               /* sum 0 reachable */

    for (idx_t v = 0; v < cnt; ++v) {
        size_t d = divs[v];
        size_t wshift = d >> 6;
        unsigned bshift = d & 63;

        /* clear tmp for this shift */
        memset(tmp, 0, words * sizeof(uint64_t));

        for (size_t w = 0; w < words; ++w) {
            uint64_t val = bits[w];
            if (!val) continue;
            size_t t = w + wshift;
            if (t < words) {
                if (bshift == 0) {
                    tmp[t] |= val;
                } else {
                    tmp[t] |= val << bshift;
                    if (t + 1 < words) tmp[t + 1] |= val >> (64 - bshift);
                }
            }
        }
        /* OR shifted bits into original set */
        for (size_t w = 0; w < words; ++w) bits[w] |= tmp[w];
    }

    /* ---------- count reachable sums, excluding 0 ---------- */
    seq_t count = 0;
    for (size_t w = 0; w < words; ++w) {
        uint64_t val = bits[w];
        if (w == 0) val &= ~1ULL;      /* exclude sum 0 */
        count += __builtin_popcountll(val);
    }

    free(bits);
    free(tmp);
    free(divs);

    A119_vals[k]  = count;
    A119_done[k]  = 1;
    return count;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    ensure_arrays(n);
    if (seq_done[n]) return seq_vals[n];

    if (n == 1) {
        seq_vals[1] = 1;
        seq_done[1] = 1;
        return 1;
    }

    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;
            if (d1 < n) {
                seq_t a_d1 = a(d1);
                seq_t A_val = get_A119347(n / d1);
                sum += A_val * a_d1;
            }
            if (d2 != d1 && d2 < n) {
                seq_t a_d2 = a(d2);
                seq_t A_val = get_A119347(n / d2);
                sum += A_val * a_d2;
            }
        }
    }

    seq_vals[n] = -sum;
    seq_done[n] = 1;
    return seq_vals[n];
}
