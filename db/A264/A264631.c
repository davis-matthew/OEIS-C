/*
 * A264631  Number of (4+1)X(n+1) arrays of permutations of 0..n*5+4 with each
 *          element having directed index change -2,0 -1,0 0,-1 or 1,1.
 * https://oeis.org/A264631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical linear recurrence for n>21: a(n) = 3*a(n-1) +36*a(n-3)
 *            -18*a(n-4) +186*a(n-5) +83*a(n-6) +144*a(n-7) + 684*a(n-8)
 *            -1872*a(n-9) +508*a(n-10) -5048*a(n-11) -3240*a(n-12) +
 *            720*a(n-13) -95040*a(n-14) +3072*a(n-15) +3456*a(n-16)
 *            +13824*a(n-18). Initial terms used for n ≤ 22: a(1) = 1, a(2)...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t initial[22] = {
    (seq_t)1UL,        (seq_t)8UL,        (seq_t)22UL,        (seq_t)108UL,
    (seq_t)684UL,      (seq_t)2736UL,     (seq_t)13556UL,     (seq_t)68972UL,
    (seq_t)316068UL,   (seq_t)1534532UL,  (seq_t)7430832UL,   (seq_t)35311040UL,
    (seq_t)170094764UL,(seq_t)817176616UL,(seq_t)3914397580ULL,(seq_t)18804795120ULL,
    (seq_t)90252696296ULL,(seq_t)433011737840ULL,(seq_t)2078662232124ULL,
    (seq_t)9975949817428ULL,(seq_t)47876003240128ULL,(seq_t)229786823307824ULL
};

static seq_t *cache = NULL;       /* memoized values  a(1) -> a(cache_len) */
static size_t cache_len = 0;      /* number of values stored (>=0) */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n=0, safe fallback */

    /* Ensure cache has at least n values, growing if necessary */
    if (cache == NULL) {
        size_t need = (n > 22) ? (size_t)n : 22;
        cache = (seq_t *)realloc(NULL, need * sizeof(seq_t));
        cache_len = need;

        /* Copy initial terms */
        for (size_t i = 0; i < 22; ++i)
            cache[i] = initial[i];

        /* Compute further terms by recurrence if needed */
        for (idx_t i = 23; i <= (idx_t)cache_len; ++i) {
            seq_t a1  = cache[(size_t)(i-2)];
            seq_t a3  = cache[(size_t)(i-4)];
            seq_t a4  = cache[(size_t)(i-5)];
            seq_t a5  = cache[(size_t)(i-6)];
            seq_t a6  = cache[(size_t)(i-7)];
            seq_t a7  = cache[(size_t)(i-8)];
            seq_t a8  = cache[(size_t)(i-9)];
            seq_t a9  = cache[(size_t)(i-10)];
            seq_t a10 = cache[(size_t)(i-11)];
            seq_t a11 = cache[(size_t)(i-12)];
            seq_t a12 = cache[(size_t)(i-13)];
            seq_t a13 = cache[(size_t)(i-14)];
            seq_t a14 = cache[(size_t)(i-15)];
            seq_t a15 = cache[(size_t)(i-16)];
            seq_t a16 = cache[(size_t)(i-17)];
            seq_t a18 = cache[(size_t)(i-19)];

            seq_t pos = 0, neg = 0;
            pos += 3 * a1;
            pos += 36 * a3;
            pos += 186 * a5;
            pos += 83 * a6;
            pos += 144 * a7;
            pos += 684 * a8;
            pos += 508 * a10;
            pos += 720 * a13;
            pos += 3072 * a15;
            pos += 3456 * a16;
            pos += 13824 * a18;

            neg += 18 * a4;
            neg += 1872 * a9;
            neg += 5048 * a11;
            neg += 3240 * a12;
            neg += 95040 * a14;

            cache[(size_t)(i-1)] = pos - neg;
        }
    } else if (cache_len < (size_t)n) {
        cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        for (idx_t i = (idx_t)cache_len + 1; i <= n; ++i) {
            seq_t a1  = cache[(size_t)(i-2)];
            seq_t a3  = cache[(size_t)(i-4)];
            seq_t a4  = cache[(size_t)(i-5)];
            seq_t a5  = cache[(size_t)(i-6)];
            seq_t a6  = cache[(size_t)(i-7)];
            seq_t a7  = cache[(size_t)(i-8)];
            seq_t a8  = cache[(size_t)(i-9)];
            seq_t a9  = cache[(size_t)(i-10)];
            seq_t a10 = cache[(size_t)(i-11)];
            seq_t a11 = cache[(size_t)(i-12)];
            seq_t a12 = cache[(size_t)(i-13)];
            seq_t a13 = cache[(size_t)(i-14)];
            seq_t a14 = cache[(size_t)(i-15)];
            seq_t a15 = cache[(size_t)(i-16)];
            seq_t a16 = cache[(size_t)(i-17)];
            seq_t a18 = cache[(size_t)(i-19)];

            seq_t pos = 0, neg = 0;
            pos += 3 * a1;
            pos += 36 * a3;
            pos += 186 * a5;
            pos += 83 * a6;
            pos += 144 * a7;
            pos += 684 * a8;
            pos += 508 * a10;
            pos += 720 * a13;
            pos += 3072 * a15;
            pos += 3456 * a16;
            pos += 13824 * a18;

            neg += 18 * a4;
            neg += 1872 * a9;
            neg += 5048 * a11;
            neg += 3240 * a12;
            neg += 95040 * a14;

            cache[(size_t)(i-1)] = pos - neg;
        }
        cache_len = (size_t)n;
    }

    return cache[(size_t)(n-1)];
}
