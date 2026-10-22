/*
 * A308236  Take apart the sides of each of the integer-sided triangles with
 *          perimeter n (at their vertices) and rearrange them orthogonally in
 *          3-space so that their endpoints coincide at a single point. a(n)
 *          is the total surface area of all rectangular prisms enclosed in
 *          this way.
 * https://oeis.org/A308236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence a(n) (OEIS A308236) by the 21‑term linear recurrence:
 * a(n) = -2a(n-1) - 2a(n-2) + a(n-3) + 7a(n-4) +10a(n-5)+7a(n-6)
 *        -5a(n-7)-17a(n-8)-19a(n-9)-9a(n-10)+9a(n-11)+19a(n-12)
 *        +17a(n-13)+5a(n-14)-7a(n-15)-10a(n-16)-7a(n-17)-a(n-18)
 *        +2a(n-19)+2a(n-20)+a(n-21)   for n>21
 * Initial values:
 * a(1)=0, a(2)=0, a(3)=6, a(4)=0, a(5)=16, a(6)=24, a(7)=62,
 * a(8)=42, a(9)=154, a(10)=130, a(11)=304, a(12)=280, a(13)=530,
 * a(14)=504, a(15)=990, a(16)=820, a(17)=1448, a(18)=1452,
 * a(19)=2260, a(20)=2040, a(21)=3318
 */
#include <stdlib.h>
#include <stdint.h>   /* for size_t if needed */

/*  static cache for the sequence values ; a(1) is stored at index 0 */
static seq_t *seq_cache = NULL;    /* 1‑based indices -> 0‑based in array */
static size_t cache_size = 0;      /* number of elements currently cached */

/*  constant initial values (22 entries so we can index 1..21 via 0..20) */
static const seq_t init_vals[22] = {
    0ULL, 0ULL, 6ULL, 0ULL, 16ULL, 24ULL, 62ULL, 42ULL,
    154ULL, 130ULL, 304ULL, 280ULL, 530ULL, 504ULL,
    990ULL, 820ULL, 1448ULL, 1452ULL, 2260ULL,
    2040ULL, 3318ULL
};

seq_t a(idx_t n)
{
    if (n == 0)               /* out of range – OEIS offset is 1 */
        return 0;

    /* ensure the cache can hold a(n) (1‑based into 0‑based array) */
    if ((size_t)n > cache_size) {
        size_t old_sz = cache_size;
        seq_t *new_cache = (seq_t *)realloc(seq_cache,
                                            (size_t)n * sizeof(seq_t));
        if (!new_cache)                 /* allocation failed */
            abort();
        seq_cache = new_cache;
        cache_size = (size_t)n;

        /* initialise known initial terms */
        if (old_sz < 21) {
            size_t i = old_sz;
            while (i < cache_size && i < 21) {
                seq_cache[i] = init_vals[i];
                ++i;
            }
        }

        /* compute further terms using the recurrence */
        for (idx_t cur = old_sz + 1; cur <= n; ++cur) {
            if (cur <= 21)          /* the first 21 values are already set */
                continue;

            /* zero‑based array index for a(cur) */
            size_t idx = (size_t)(cur - 1);

            __int128 val = 0;       /* 128‑bit temporary for signed arithmetic */

            val  += -2  * (__int128)seq_cache[idx - 1];
            val  += -2  * (__int128)seq_cache[idx - 2];
            val  +=  1  * (__int128)seq_cache[idx - 3];
            val  +=  7  * (__int128)seq_cache[idx - 4];
            val  += 10  * (__int128)seq_cache[idx - 5];
            val  +=  7  * (__int128)seq_cache[idx - 6];
            val  += -5  * (__int128)seq_cache[idx - 7];
            val  += -17 * (__int128)seq_cache[idx - 8];
            val  += -19 * (__int128)seq_cache[idx - 9];
            val  += -9  * (__int128)seq_cache[idx -10];
            val  +=  9  * (__int128)seq_cache[idx -11];
            val  += 19  * (__int128)seq_cache[idx -12];
            val  += 17  * (__int128)seq_cache[idx -13];
            val  +=  5  * (__int128)seq_cache[idx -14];
            val  += -7  * (__int128)seq_cache[idx -15];
            val  += -10 * (__int128)seq_cache[idx -16];
            val  += -7  * (__int128)seq_cache[idx -17];
            val  += -1  * (__int128)seq_cache[idx -18];
            val  +=  2  * (__int128)seq_cache[idx -19];
            val  +=  2  * (__int128)seq_cache[idx -20];
            val  +=  1  * (__int128)seq_cache[idx -21];

            seq_cache[idx] = (seq_t)val;
        }
    }

    return seq_cache[n - 1];
}
