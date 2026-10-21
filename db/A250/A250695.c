/*
 * A250695  Number of (4+1)X(n+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 10, u64 n <= 27, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for OEIS A250695
 * a(n) = 30·a(n-1) −415·a(n-2) +3514·a(n-3) −20386·a(n-4) +85924·a(n-5)
 *        −272198·a(n-6) +661180·a(n-7) −1244717·a(n-8) +1822478·a(n-9)
 *        −2068955·a(n-10)+1802474·a(n-11)−1181760·a(n-12)+563888·a(n-13)
 *        −184752·a(n-14)+37152·a(n-15) −3456·a(n-16).
 * Initial values (n = 1 … 16):
 * 11736, 76320, 383440, 1670016, 6671458, 25235016, 92169818,
 * 329292956, 1160994284, 4064711976, 14193722396, 49590850136,
 * 173757950350, 611608432408, 2165540377158, 7721145553812
 */

#include <stdlib.h>

static const seq_t init[17] = {
    0ULL,                                      /* unused at index 0 */
    (seq_t)11736ULL,      /* a(1)  */
    (seq_t)76320ULL,      /* a(2)  */
    (seq_t)383440ULL,     /* a(3)  */
    (seq_t)1670016ULL,    /* a(4)  */
    (seq_t)6671458ULL,    /* a(5)  */
    (seq_t)25235016ULL,   /* a(6)  */
    (seq_t)92169818ULL,   /* a(7)  */
    (seq_t)329292956ULL,  /* a(8)  */
    (seq_t)1160994284ULL, /* a(9)  */
    (seq_t)4064711976ULL, /* a(10) */
    (seq_t)14193722396ULL,/* a(11) */
    (seq_t)49590850136ULL,/* a(12) */
    (seq_t)173757950350ULL,/* a(13) */
    (seq_t)611608432408ULL,/* a(14) */
    (seq_t)2165540377158ULL,/* a(15) */
    (seq_t)7721145553812ULL /* a(16) */
};

static const idx_t  pos_off[8] = {1,3,5,7,9,11,13,15};
static const idx_t  neg_off[8] = {2,4,6,8,10,12,14,16};
static const seq_t pos_coef[8] = {
    (seq_t)30ULL, (seq_t)3514ULL, (seq_t)85924ULL,
    (seq_t)661180ULL, (seq_t)1822478ULL, (seq_t)1802474ULL,
    (seq_t)563888ULL, (seq_t)37152ULL
};
static const seq_t neg_coef[8] = {
    (seq_t)415ULL, (seq_t)20386ULL, (seq_t)272198ULL,
    (seq_t)1244717ULL, (seq_t)2068955ULL, (seq_t)1181760ULL,
    (seq_t)184752ULL, (seq_t)3456ULL
};

seq_t a(idx_t n)
{
    /* n must be >= 1; for n < 1 the sequence is undefined */
    if (n < 1) return (seq_t)0;

    /* static cache for memoisation: indices 0 .. current_max */
    static seq_t *cache = NULL;
    static idx_t current_max = 0;   /* largest index stored */

    /* initialise cache with the first 16 terms if needed */
    if (cache == NULL) {
        cache = (seq_t *)malloc(17 * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure */
        for (idx_t i = 0; i <= 16; ++i) cache[i] = init[i];
        current_max = 16;
    }

    /* grow cache if n is beyond current_max */
    if (n > current_max) {
        size_t new_size = (size_t)(n + 1);   /* indices 0 .. n */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failure */
        cache = tmp;

        for (idx_t i = current_max + 1; i <= n; ++i) {
            if (i <= 16) {
                /* safety: already stored in init */
                cache[i] = init[i];
                continue;
            }
            seq_t sum_pos = 0;
            seq_t sum_neg = 0;

            /* positive contributions */
            for (int k = 0; k < 8; ++k) {
                sum_pos += cache[i - pos_off[k]] * pos_coef[k];
            }

            /* negative contributions */
            for (int k = 0; k < 8; ++k) {
                sum_neg += cache[i - neg_off[k]] * neg_coef[k];
            }

            cache[i] = sum_pos - sum_neg;
        }
        current_max = n;
    }

    return cache[n];
}
