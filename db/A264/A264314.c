/*
 * A264314  Number of (1+1)X(n+1) arrays of permutations of 0..n*2+1 with each
 *          element having directed index change 0,0 0,1 1,0 -1,-2 or 0,2.
 * https://oeis.org/A264314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 2*a(n-3) + 5*a(n-4) - a(n-6) -
 *            8*a(n-7) - 3*a(n-8) + 6*a(n-9) - a(n-10) - 2*a(n-11) - a(n-12) +
 *            a(n-14) for n > 14 Initial values: a(1)=1, a(2)=6, a(3)=20,
 *            a(4)=52, a(5)=140, a(6)=392, a(7)=1117, a(8)=3155, a(9)=8845,
 *            a(10)=24812, a(11)=69780, a(12)=196280, a(13)=551796...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t base_vals[15] = {
    0ULL,         /* dummy for index 0 */
    1ULL, 6ULL, 20ULL, 52ULL, 140ULL, 392ULL,
    1117ULL, 3155ULL, 8845ULL, 24812ULL, 69780ULL,
    196280ULL, 551796ULL, 1550968ULL
};

#if defined(SEQ_T_MAX_N)
#define MAX_N SEQ_T_MAX_N   /* optional override for large arrays */
#else
#define MAX_N 10000          /* sufficient for typical usage */
#endif

static seq_t cache[MAX_N + 1];
static size_t cache_len = 0;

/* Compute a(n) using the known recurrence, storing intermediate
   results in a cache to support arbitrary call order. */
seq_t a(idx_t n)
{
    /* trivial offset handling: n==0 is undefined for this sequence,
       but returning 0 avoids out-of-bounds. */
    if (n == 0) return 0;

    if (n <= 14)            /* use the pre‑computed initial values */
        return base_vals[(size_t)n];

    /* initialise cache on the first call that requires recurrence */
    if (cache_len == 0) {
        size_t i;
        for (i = 1; i <= 14; ++i)
            cache[i] = base_vals[i];
        cache_len = 14;
    }

    /* compute values up to n if not already cached */
    if ((size_t)n > cache_len) {
        idx_t k;
        for (k = (idx_t)(cache_len + 1); k <= n; ++k) {
            seq_t pos = 2 * cache[(size_t)(k - 1)];
            pos += cache[(size_t)(k - 2)];
            pos += 2 * cache[(size_t)(k - 3)];
            pos += 5 * cache[(size_t)(k - 4)];
            pos += 6 * cache[(size_t)(k - 9)];
            pos += cache[(size_t)(k - 14)];

            seq_t neg = 1 * cache[(size_t)(k - 6)];
            neg += 8 * cache[(size_t)(k - 7)];
            neg += 3 * cache[(size_t)(k - 8)];
            neg += 1 * cache[(size_t)(k - 10)];
            neg += 2 * cache[(size_t)(k - 11)];
            neg += 1 * cache[(size_t)(k - 12)];

            cache[(size_t)k] = pos - neg;
        }
        cache_len = (size_t)n;
    }

    return cache[(size_t)n];
}
