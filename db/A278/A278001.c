/*
 * A278001  Number of nX3 0..2 arrays with every element plus 1 mod 3 equal to
 *          some element at offset (-1,-1) (-1,0) (-1,1) (0,-1) (0,1) (1,-1)
 *          or (1,0), with upper left element zero.
 * https://oeis.org/A278001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18*a(n-1)+21*a(n-2)+15*a(n-3)-530*a(n-4)-562*a(n-5)
 *            +986*a(n-6)+5831*a(n-7)-1452*a(n-8)-8962*a(n-9)
 *            -19556*a(n-10)+24096*a(n-11)+21600*a(n-12)+12480*a(n-13)
 *            -45824*a(n-14)+3456*a(n-15)-768*a(n-16) Initial values:
 *            a(1)=0,a(2)=52,a(3)=866,a(4)=16583,a(5)=316432,
 *            a(6)=6031565,a(7)=114975158,a...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of computed terms (1‑based index stored at position n‑1). */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialise the first 16 terms once, on first call. */
    if (cache_len == 0) {
        cache_len = 16;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed */

        cache[0]  = (seq_t)0;
        cache[1]  = (seq_t)52;
        cache[2]  = (seq_t)866;
        cache[3]  = (seq_t)16583;
        cache[4]  = (seq_t)316432;
        cache[5]  = (seq_t)6031565;
        cache[6]  = (seq_t)114975158;
        cache[7]  = (seq_t)2191741025;
        cache[8]  = (seq_t)41780404363ULL;
        cache[9]  = (seq_t)796445224559ULL;
        cache[10] = (seq_t)15182359059263ULL;
        cache[11] = (seq_t)289416039476474ULL;
        cache[12] = (seq_t)5517037468209865ULL;
        cache[13] = (seq_t)105169369667173021ULL;
        cache[14] = (seq_t)2004807177668663124ULL;

        /* a(16) : 38216943130107881653 = (2 << 64) + 1323454982688778421 */
        seq_t a16;
        if (sizeof(seq_t) > 8) {   /* 128‑bit build */
            a16 = ((seq_t)2 << 64) | (seq_t)1323454982688778421ULL;
        } else {                    /* 64‑bit or smaller – keep low part only */
            a16 = (seq_t)1323454982688778421ULL;
        }
        cache[15] = a16;
    }

    if (n <= cache_len)
        return cache[n - 1];

    /* Extend cache up to n. */
    idx_t new_len = n;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return (seq_t)0;   /* allocation failed */
    cache = tmp;

    /* Recurrence coefficients. */
    static const int coeffs[16] = {
        18, 21, 15, -530, -562, 986, 5831, -1452,
        -8962, -19556, 24096, 21600, 12480, -45824, 3456, -768
    };

    /* Compute missing terms. */
    for (idx_t k = cache_len; k < new_len; ++k) {
        seq_t sum = (seq_t)0;
        for (int i = 0; i < 16; ++i) {
            seq_t term = cache[k - 1 - i];
            int coeff = coeffs[i];
            if (coeff >= 0) {
                sum += term * (seq_t)coeff;
            } else {
                sum -= term * (seq_t)(-coeff);
            }
        }
        cache[k] = sum;
    }

    cache_len = new_len;
    return cache[n - 1];
}
